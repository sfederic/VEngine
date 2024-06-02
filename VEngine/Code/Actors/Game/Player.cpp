#include "vpch.h"
#include "Player.h"
#include "Core/Camera.h"
#include "Core/Core.h"
#include "Core/Input.h"
#include "Core/VMath.h"
#include "Core/Log.h"
#include "Core/Timer.h"
#include "Physics/Raycast.h"
#include "Audio/MaterialAudioType.h"
#include "Actors/Game/NPC.h"
#include "Actors/Game/InspectionTrigger.h"
#include "Actors/Game/FenceActor.h"
#include "Actors/Game/PlayerCameraTrigger.h"
#include "Grid.h"
#include "GridActor.h"
#include "Components/Game/DialogueComponent.h"
#include "Components/MeshComponent.h"
#include "Render/RastState.h"
#include "Render/BlendStates.h"
#include "Render/Material.h"
#include "UI/UISystem.h"
#include "UI/Game/DialogueWidget.h"
#include "UI/Game/InteractWidget.h"
#include "Gameplay/GameUtils.h"

//Distance the player can ray/box cast to a grid actor to link to.
constexpr float linkDistance = 5.f;

Player::Player()
{
	SetEmptyRootComponent();

	camera = CreateComponent<CameraComponent>("Camera");
	rootComponent->AddChild(camera);

	nextPos = XMVectorZero();
	nextRot = XMVectorZero();

	_mesh = CreateComponent("Mesh", MeshComponent("char.vmesh", "test.png"));
	rootComponent->AddChild(_mesh);

	dialogueComponent = DialogueComponent::system.Add("Dialogue", this);

	linkEffectMesh = CreateComponent<MeshComponent>("LinkEffectMesh");

	camera->SetLocalPosition(cameraStartingLocalPosition);
}

void Player::Create()
{
	__super::Create();

	_mesh->SetRenderStatic(false);

	linkEffectMesh->SetActive(true);
	linkEffectMesh->SetMeshFilename("cube.vmesh");
	linkEffectMesh->SetTexture("UI/spellbinding_circle.png");
	linkEffectMesh->SetCollisionLayer(CollisionLayers::None);
	linkEffectMesh->SetShaderItem("DefaultClip");
	linkEffectMesh->SetBlendState(BlendStates::Transparent);
	linkEffectMesh->SetAlpha(0.f);
	linkEffectMesh->castsShadow = false;

	cameraLinkActiveLocalPosition = XMVectorSet(1.25f, 0.55f, -0.75f, 1.f);

	moveSpeed = 5.35f;
	rotateSpeed = 4.75f;
}

void Player::Start()
{
	__super::Start();

	nextPos = GetPositionV();
	nextRot = GetRotationV();

	SetGridIndices();

	camera->SetTargetActor(this);
	nextCameraFOV = camera->GetFOV();
	camera->SetAsActiveCamera();
	nextCameraPosition = camera->GetLocalPositionV();

	//Setup widgets
	interactWidget = UISystem::CreateWidget<InteractWidget>();
}

void Player::End()
{
	__super::End();

	previousHitTransparentActors.clear();
}

void Player::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (gameOver)
	{
		return;
	}

	if (healthPoints <= 0)
	{
		//GameUtils::TriggerGameOver();
	}

	OnMoveAndRotateEnd();

	MoveLinkedGridActor();
	RotateLinkedGridActor();
	LinkToGridActor();

	ToggleGrid();

	MovementInput(deltaTime);
	RotationInput(deltaTime);

	PrimaryAction();

	LerpPlayerCameraFOV(deltaTime);
	MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	dialogueComponent->SetPosition(GetHomogeneousPositionV());

	//Lerp actor position and rotation
	if (inHop)
	{
		SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextHopPos, deltaTime, moveSpeed));

		if (VMath::VecEqual(GetPositionV(), nextHopPos))
		{
			inHop = false;
			hopLanding = true;
		}
	}
	else
	{
		SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, moveSpeed));

		if (hopLanding && VMath::VecEqual(GetPositionV(), nextPos))
		{
			GameUtils::PlayAudioOneShot("jumpland.wav");
			hopLanding = false;
		}
	}

	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, rotateSpeed));

	//Lerp camera position
	camera->SetLocalPosition(
		VMath::VectorConstantLerp(camera->GetLocalPositionV(), nextCameraPosition, deltaTime, 14.f));

	UpdateLinkEffectMesh();
}

Properties Player::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

XMVECTOR Player::GetMeshForward()
{
	return _mesh->GetForwardVectorV();
}

XMVECTOR Player::GetMeshRight()
{
	return _mesh->GetRightVectorV();
}

bool Player::CheckAttackPositionAgainstUnitDirection(Unit* unit)
{
	if (unit->attackDirections & AttackDirection::All)
	{
		return true;
	}

	auto playerForward = _mesh->GetForwardVectorV();
	VMath::RoundVector(playerForward);

	auto unitForward = unit->GetForwardVectorV();
	VMath::RoundVector(unitForward);

	auto unitRight = unit->GetRightVectorV();
	VMath::RoundVector(unitRight);

	if (DirectX::XMVector4Equal(unitForward, -playerForward))
	{
		if (unit->attackDirections & AttackDirection::Front)
		{
			return true;
		}
	}
	else if (DirectX::XMVector4Equal(unitForward, playerForward))
	{
		if (unit->attackDirections & AttackDirection::Back)
		{
			return true;
		}
	}
	else if (DirectX::XMVector4Equal(unitRight, playerForward))
	{
		if (unit->attackDirections & AttackDirection::Right)
		{
			return true;
		}
	}
	else if (DirectX::XMVector4Equal(-unitRight, playerForward))
	{
		if (unit->attackDirections & AttackDirection::Left)
		{
			return true;
		}
	}

	return false;
}

void Player::ToggleGrid()
{
	if (IsInInteraction())
	{
		return;
	}

	if (Input::GetKeyUp("ToggleGrid"))
	{
		GameUtils::PlayAudioOneShot("sword_sheathe.wav");

		auto grid = Grid::system.GetOnlyActor();
		if (grid->IsLerpingIn())
		{
			grid->SetLerpOut();
		}
		else
		{
			grid->SetLerpIn();
		}
	}
}

void Player::HighlightLinkableGridActor()
{
	if (!CheckIfMovementAndRotationStopped())
	{
		return;
	}
	if (isInputLinkedToGridActor)
	{
		highlightedGridActor = nullptr;
		return;
	}

	HitResult hit(this);
	const auto playerPos = GetPositionV();
	const auto end = playerPos + (GetMeshForward() * linkDistance);
	if (Physics::OrientedBoxCast(hit, playerPos, end, XMFLOAT2(0.25f, 0.25f), false, false))
	{
		ResetHighlightedActor();

		auto gridActor = hit.GetClosestHitActorAs<GridActor>(playerPos);
		if (gridActor)
		{
			highlightedGridActor = gridActor;

			gridActor->OnPlayerLinkHover();

			if (gridActor->IsLinked())
			{
				return;
			}
			if (!gridActor->CanBeMovedInLink() && !gridActor->CanBeRotatedInLink())
			{
				return;
			}

			if (!CheckIfMeshCanBeLinkedTo(gridActor))
			{
				return;
			}

			EnableLinkEffectMeshForHover(&highlightedGridActor->GetMesh());

			previousHighlightedGridActor = highlightedGridActor;
		}
		else
		{
			linkEffectMeshSetAlpha = false;
			ResetHighlightedActor();
		}
	}
	else
	{
		linkEffectMeshSetAlpha = false;
		ResetHighlightedActor();
	}
}

void Player::PrimaryAction()
{
	if (!CheckIfMovementAndRotationStopped())
	{
		return;
	}

	if (Input::GetKeyUp("Interact"))
	{
		if (!inInspection)
		{
			for (auto& inspectionTrigger : InspectionTrigger::system.GetActors())
			{
				if (inspectionTrigger->ContainsPlayer())
				{
					inspectionTrigger->Inspect();
					inInspection = true;
					return;
				}
			}
		}

		if (inInteraction)
		{
			//End interact with GridActor
			if (gridActorInteractingWith == nullptr)
			{
				return;
			}

			gridActorInteractingWith->EndInteract();
			gridActorInteractingWith = nullptr;

			interactWidget->RemoveFromViewport();
			inInteraction = false;

			SetDefaultCameraFOV();

			return;
		}

		if (inInspection)
		{
			inInspection = false;
			ResetCameraPosAndTargetToPlayer();
			interactWidget->RemoveFromViewport();
			return;
		}

		HitResult hit(this);
		auto meshForward = _mesh->GetForwardVectorV();
		auto center = GetPositionV();
		auto boxCastOrigin = center + meshForward;
		if (Physics::SimpleBoxCast(boxCastOrigin, XMFLOAT3(0.25f, 0.25f, 0.25f), hit, false, true))
		{
			hit.hitActor = hit.GetClosestHitActor(center);
			Log("Player interact: %s", hit.hitActor->GetName().c_str());

			if (DialogueCheck(hit.hitActor)) {}
			else if (QuickTalkCheck(hit.hitActor)) {}
			else if (InteractCheck(hit.hitActor)) {}
		}
	}
}

void Player::LerpPlayerCameraFOV(float deltaTime)
{
	if (camera->GetFOV() != nextCameraFOV)
	{
		camera->SetFOV(std::lerp(camera->GetFOV(), nextCameraFOV, 4.f * deltaTime));
	}
}

bool Player::DialogueCheck(Actor* hitActor)
{
	if (inConversation)
	{
		if (!currentlyActiveDialogueComponent->NextLine())
		{
			//End dialogue
			inConversation = false;
			SetDefaultCameraFOV();
			Camera::SetActiveCameraTarget(this);
			currentlyActiveDialogueComponent = nullptr;
			return true;
		}
		else
		{
			if (!currentlyActiveDialogueComponent->ShowTextAtActor())
			{
				//Exist out of dialogue loop if next line not found.
				inConversation = false;
				currentlyActiveDialogueComponent = nullptr;
				//GameUtils::SetActiveCameraTargetAndZoomOut(this);
			}

			return false;
		}
	}
	else
	{
		NPC* npc = dynamic_cast<NPC*>(hitActor);
		if (npc)
		{
			currentlyActiveDialogueComponent = npc->GetDialogueComponent();
			if (currentlyActiveDialogueComponent->dialogue.filename.empty())
			{
				return false;
			}

			inConversation = true;

			//start dialogue
			currentlyActiveDialogueComponent->ShowTextAtActor();

			SetZoomedInCameraFOV();

			return true;
		}
	}

	return false;
}

//Note: Default blend state needs to already be set for the mesh.
void Player::MakeOccludingMeshBetweenCameraAndPlayerTransparent()
{
	const auto cameraToPlayerDir = XMVector3Normalize(camera->GetWorldPositionV() - GetPositionV());
	const auto start = GetPositionV() + cameraToPlayerDir;
	HitResult hit(this);
	if (Physics::OrientedBoxCast(hit, start, camera->GetWorldPositionV(), XMFLOAT2(0.25f, 0.25f), true, false))
	{
		for (Actor* actor : hit.hitActors)
		{
			if (actor->CanBeTransparentlyOccluded())
			{
				previousHitTransparentActors.insert(actor);

				auto lMesh = actor->GetFirstComponentOfTypeAllowNull<MeshComponent>();
				if (lMesh && lMesh->transparentOcclude)
				{
					float alpha = lMesh->GetAlpha();
					if (alpha > 0.33f)
					{
						alpha -= Core::GetDeltaTime() * 3.f;
						lMesh->SetAlpha(alpha);
					}
				}
			}
		}
	}

	std::set<Actor*> actorsToRemove;

	for (auto it = previousHitTransparentActors.begin(); it != previousHitTransparentActors.end(); it++)
	{
		if (hit.FindHitActor(*(it)) == nullptr)
		{
			auto lMesh = (*it)->GetFirstComponentOfTypeAllowNull<MeshComponent>();
			if (lMesh && lMesh->transparentOcclude)
			{
				float alpha = _mesh->GetAlpha();
				if (alpha < 1.f)
				{
					alpha += Core::GetDeltaTime() * 1.5f;
					lMesh->SetAlpha(alpha);
				}

				if (alpha >= 1.f)
				{
					actorsToRemove.erase(*(it));
				}
			}
		}
	}

	for (auto actor : actorsToRemove)
	{
		auto it = previousHitTransparentActors.find(actor);
		if (it != previousHitTransparentActors.end())
		{
			previousHitTransparentActors.erase(actor);
		}
	}
}

bool Player::CheckIfPlayerIsInsidePlayerCameraTriggerAndReset()
{
	for (auto& playerCameraTrigger : PlayerCameraTrigger::system.GetActors())
	{
		if (playerCameraTrigger->ContainsPlayer())
		{
			playerCameraTrigger->ResetCameraPosAndTarget();
			return true;
		}
	}
	return false;
}

bool Player::QuickTalkCheck(Actor* hitActor)
{
	if (!inConversation)
	{
		auto npc = dynamic_cast<NPC*>(hitActor);
		if (npc)
		{
			if (npc->isInteractable)
			{
				npc->QuickTalkTo();
				return true;
			}
		}
	}

	return false;
}

void Player::SetQuickThought(std::wstring_view text)
{
	dialogueComponent->dialogueWidget->ResetAllValues();
	dialogueComponent->dialogueWidget->SetText(text);
	dialogueComponent->dialogueWidget->DeleteOnTextProgressEnd();
	dialogueComponent->dialogueWidget->AddToViewport();
}

void Player::SetLinkEffectMeshForTransformativeGridActors(MeshComponent& mesh)
{
	linkEffectMesh->SetVisibility(true);
	linkEffectMesh->SetWorldScale(mesh.GetWorldScaleV() * 1.05f);
	linkEffectMesh->SetWorldPosition(mesh.GetWorldPositionV());
	linkEffectMesh->SetRastState(mesh.GetRastState().GetName());
	linkEffectMesh->SetMeshFilename(mesh.GetMeshFilename());
	linkEffectMesh->ReCreate();
}

bool Player::CombatInteractCheck(Actor* actorToCheck)
{
	auto gridActor = dynamic_cast<GridActor*>(actorToCheck);
	if (gridActor)
	{
		if (gridActor->isInteractable)
		{
			gridActor->Interact();
			gridActorInteractingWith = gridActor;

			return true;
		}
	}

	return false;
}

bool Player::InteractCheck(Actor* hitActor)
{
	if (!inConversation && !isInputLinkedToGridActor)
	{
		auto gridActor = dynamic_cast<GridActor*>(hitActor);
		if (gridActor)
		{
			if (gridActor->IsInspectable())
			{
				inInspection = true;

				gridActor->Inspect();

				if (!gridActor->GetInteractText().empty())
				{
					interactWidget->interactText = gridActor->GetInteractText();
					interactWidget->AddToViewport();
				}

				return true;
			}

			if (gridActor->isInteractable)
			{
				gridActor->Interact();
				gridActorInteractingWith = gridActor;
				return true;
			}
		}
	}

	return false;
}

void Player::InteractInfoToWidgetCheck()
{
	HitResult hit(this);
	if (Physics::Raycast(hit, GetPositionV(), GetMeshForward(), 1.f))
	{
		auto npc = hit.GetHitActorAs<NPC>();
		if (npc)
		{
			interactWidget->SetWorldPosition(GetHomogeneousPositionV());
			interactWidget->interactText = L"Talk";
			interactWidget->AddToViewport();
			return;
		}

		auto gridActor = hit.GetHitActorAs<GridActor>();
		if (gridActor)
		{
			if (!gridActor->GetInteractText().empty())
			{
				interactWidget->SetWorldPosition(GetHomogeneousPositionV());
				interactWidget->interactText = gridActor->GetInteractText();
				interactWidget->AddToViewport();
			}
		}
	}
	else
	{
		interactWidget->RemoveFromViewport();
	}
}

bool Player::AttackGridActorBasedOnNode()
{
	auto grid = Grid::system.GetFirstActor();
	const int attackNodeIndexX = xIndex + GetForwardVector().x;
	const int attackNodeIndexY = yIndex + GetForwardVector().z;
	auto attackNode = grid->GetNodeLimit(attackNodeIndexX, attackNodeIndexY);

	for (auto gridActor : World::GetAllActorsOfTypeInWorld<GridActor>())
	{
		if (gridActor->GetCurrentNode()->Equals(attackNode))
		{
			gridActor->InflictDamage(1);
			return true;
		}
	}

	return false;
}

void Player::LinkToGridActor()
{
	if (IsInInteraction())
	{
		return;
	}

	if (!CheckIfMovementAndRotationStopped())
	{
		return;
	}

	if (isInputLinkedToGridActor) //Cancel
	{
		if (Input::GetKeyUp("Unlink"))
		{
			linkEffectMeshSetAlpha = false;
			ResetLinkedGridActor();
		}
	}
	else if (!isInputLinkedToGridActor && Input::GetKeyUp("Link"))
	{
		interactWidget->RemoveFromViewport();

		HitResult hit(this);
		const auto playerPos = GetPositionV();
		const auto end = playerPos + (GetMeshForward() * 5.f);
		if (Physics::OrientedBoxCast(hit, playerPos, end, XMFLOAT2(0.25f, 0.25f), false, false))
		{
			auto closestActorToLinkTo = hit.GetClosestHitActorAs<GridActor>(playerPos);
			if (closestActorToLinkTo == nullptr)
			{
				return;
			}

			if (!CheckIfMeshCanBeLinkedTo(closestActorToLinkTo))
			{
				return;
			}

			//This raycast is to make sure the player is not standing on the same actor it's linking to
			//to avoid potentially rotating the linked actor and the player being stuck in mid-air.
			HitResult sameActorHit(this);
			if (Physics::Raycast(sameActorHit, GetPositionV(), -VMath::GlobalUpVector(), 5.f))
			{
				auto gridActorUnderneathPlayer = sameActorHit.GetHitActorAs<GridActor>();
				if (gridActorUnderneathPlayer)
				{
					if (gridActorUnderneathPlayer == closestActorToLinkTo)
					{
						Camera::GetActiveCamera().SetShakeLevel(0.3f);
						Log("Cannot link to [%s], player is standing on it.", gridActorUnderneathPlayer->GetName().c_str());
						return;
					}

					if (!CheckIfMeshCanBeLinkedTo(closestActorToLinkTo))
					{
						return;
					}
				}
			}

			SetLinkedGridActor(*closestActorToLinkTo);
		}
	}
}

void Player::MoveLinkedGridActor()
{
	if (!CheckIfMovementAndRotationStopped())
	{
		return;
	}

	if (linkedGridActor == nullptr || !linkedGridActor->CheckMovementAndRotationStopped())
	{
		return;
	}

	const auto checkLinkMovement = [](bool canBeMoved) -> bool
		{
			if (!canBeMoved)
			{
				Camera::GetActiveCamera().SetShakeLevel(0.25f);
				return false;
			}
			return true;
		};

	if (Input::GetKeyUp("MoveForward"))
	{
		if (checkLinkMovement(linkedGridActor->CanBeMovedInLink()))
		{
			if (linkedGridActor->IsNextMoveAxisValid(GetForwardVectorV()))
			{
				linkedGridActor->AddNextPosition(GetForwardVectorV());
				if (linkedGridActor->CheckNextNodeMoveIsValid(GetForwardVectorV()))
				{
					linkedGridActor->OnLinkMove();

					//There are some GridActors that destroy themselves on move. Re-check for nullptr.
					//This check is in all movement and rotation virtual calls on a GridActor from player.
					if (linkedGridActor)
					{
						linkedGridActor->OnLinkMoveForward();
					}
				}
			}
			else
			{
				Camera::GetActiveCamera().SetShakeLevel(0.25f);
			}
		}
	}
	else if (Input::GetKeyUp("MoveBack"))
	{
		if (checkLinkMovement(linkedGridActor->CanBeMovedInLink()))
		{
			if (linkedGridActor->IsNextMoveAxisValid(-GetForwardVectorV()))
			{
				linkedGridActor->AddNextPosition(-GetForwardVectorV());
				if (linkedGridActor->CheckNextNodeMoveIsValid(-GetForwardVectorV()))
				{
					linkedGridActor->OnLinkMove();

					if (linkedGridActor == nullptr)
					{
						return;
					}

					linkedGridActor->OnLinkMoveBack();

					//Backwards linked grid actor movement needs to be able to rotate the player and camera
					//to face the new positional orientation of the linked grid actor. This eases up the
					//camera controls a bit and keeps the controller/keyboard input constant.

					//@Todo: I'd like this "is vector left or right of" code to be a VMath function.
					const auto delta = XMVector3Normalize(linkedGridActor->GetPositionV() - GetPositionV());
					const auto cross = XMVector3Cross(delta, GetForwardVectorV());

					float rotationIncrementAngle = 0.f;
					if (cross.m128_f32[1] > 0.f) //GridActor is to the right
					{
						rotationIncrementAngle = -90.f;
					}
					else //GridActor is to the left
					{
						rotationIncrementAngle = 90.f;
					}

					const float angleBetweenGridActorAndPlayer =
						XMConvertToDegrees(XMVector3AngleBetweenNormals(
							XMVector3Normalize(linkedGridActor->GetPositionV() - GetPositionV()),
							GetForwardVectorV()).m128_f32[0]);
					if (angleBetweenGridActorAndPlayer > 89.f)
					{
						const float angle = DirectX::XMConvertToRadians(rotationIncrementAngle);
						nextRot = XMQuaternionMultiply(nextRot, DirectX::XMQuaternionRotationAxis(VMath::GlobalUpVector(), angle));
					}
				}
			}
			else
			{
				Camera::GetActiveCamera().SetShakeLevel(0.25f);
			}
		}
	}
	else if (Input::GetKeyUp("MoveLeft"))
	{
		if (checkLinkMovement(linkedGridActor->CanBeMovedInLink()))
		{
			if (linkedGridActor->IsNextMoveAxisValid(-GetRightVectorV()))
			{
				linkedGridActor->AddNextPosition(-GetRightVectorV());
				if (linkedGridActor->CheckNextNodeMoveIsValid(-GetRightVectorV()))
				{
					linkedGridActor->OnLinkMove();
					if (linkedGridActor)
					{
						linkedGridActor->OnLinkMoveLeft();
					}
				}
			}
			else
			{
				Camera::GetActiveCamera().SetShakeLevel(0.25f);
			}
		}
	}
	else if (Input::GetKeyUp("MoveRight"))
	{
		if (checkLinkMovement(linkedGridActor->CanBeMovedInLink()))
		{
			if (linkedGridActor->IsNextMoveAxisValid(GetRightVectorV()))
			{
				linkedGridActor->AddNextPosition(GetRightVectorV());
				if (linkedGridActor->CheckNextNodeMoveIsValid(GetRightVectorV()))
				{
					linkedGridActor->OnLinkMove();
					if (linkedGridActor)
					{
						linkedGridActor->OnLinkMoveRight();
					}
				}
			}
			else
			{
				Camera::GetActiveCamera().SetShakeLevel(0.25f);
			}
		}
	}
}

//Note: If a GridActor isn't aligned to the player's positional axis (i.e. if it's off on the x-axis
//by 0.5 compared to Player), the GridActor won't rotate properly because it's working off of the Player's axis.
void Player::RotateLinkedGridActor()
{
	if (linkedGridActor == nullptr || !linkedGridActor->CheckMovementAndRotationStopped())
	{
		return;
	}

	const auto checkLinkRotation = [](bool canBeRotated) -> bool
		{
			if (!canBeRotated)
			{
				Camera::GetActiveCamera().SetShakeLevel(0.25f);
				return false;
			}
			return true;
		};

	constexpr float angleIncrement = 90.f;

	if (Input::GetKeyUp("RotateRight"))
	{
		if (checkLinkRotation(linkedGridActor->CanBeRotatedInLink()))
		{
			if (linkedGridActor->CanBeRotatedRollZAxis())
			{
				linkedGridActor->SetNextRot(VMath::AddRotationAngle(linkedGridActor->GetRotationV(),
					VMath::GlobalForwardVector(), -angleIncrement));
				if (!linkedGridActor->CheckNextRotationBoundsIntersect())
				{
					linkedGridActor->OnLinkRotate();
					if (linkedGridActor)
					{
						linkedGridActor->OnLinkRotateRight();
					}
				}
				else
				{
					linkedGridActor->SetNextRot(linkedGridActor->GetRotationV());
					Camera::GetActiveCamera().SetShakeLevel(0.25f);
				}
			}
			else if (linkedGridActor->CanBeRotatedYawYAxis())
			{
				linkedGridActor->SetNextRot(VMath::AddRotationAngle(linkedGridActor->GetRotationV(),
					VMath::GlobalUpVector(), -angleIncrement));
				if (!linkedGridActor->CheckNextRotationBoundsIntersect())
				{
					linkedGridActor->OnLinkRotate();

					if (linkedGridActor)
					{
						linkedGridActor->OnLinkRotateRight();
					}
				}
				else
				{
					linkedGridActor->SetNextRot(linkedGridActor->GetRotationV());
					Camera::GetActiveCamera().SetShakeLevel(0.25f);
				}
			}
		}
	}
	else if (Input::GetKeyUp("RotateLeft"))
	{
		if (checkLinkRotation(linkedGridActor->CanBeRotatedInLink()))
		{
			if (linkedGridActor->CanBeRotatedRollZAxis())
			{
				linkedGridActor->SetNextRot(VMath::AddRotationAngle(linkedGridActor->GetRotationV(),
					VMath::GlobalForwardVector(), angleIncrement));
				if (!linkedGridActor->CheckNextRotationBoundsIntersect())
				{
					linkedGridActor->OnLinkRotate();
					if (linkedGridActor)
					{
						linkedGridActor->OnLinkRotateLeft();
					}
				}
				else
				{
					linkedGridActor->SetNextRot(linkedGridActor->GetRotationV());
					Camera::GetActiveCamera().SetShakeLevel(0.25f);
				}
			}
			if (linkedGridActor->CanBeRotatedYawYAxis())
			{
				linkedGridActor->SetNextRot(VMath::AddRotationAngle(linkedGridActor->GetRotationV(),
					VMath::GlobalUpVector(), angleIncrement));
				if (!linkedGridActor->CheckNextRotationBoundsIntersect())
				{
					linkedGridActor->OnLinkRotate();
					if (linkedGridActor)
					{
						linkedGridActor->OnLinkRotateLeft();
					}
				}
				else
				{
					linkedGridActor->SetNextRot(linkedGridActor->GetRotationV());
					Camera::GetActiveCamera().SetShakeLevel(0.25f);
				}
			}
		}
	}
	else if (Input::GetKeyUp("RotateDown"))
	{
		if (checkLinkRotation(linkedGridActor->CanBeRotatedInLink()))
		{
			if (linkedGridActor->CanBeRotatedPitchXAxis())
			{
				linkedGridActor->SetNextRot(VMath::AddRotationAngle(linkedGridActor->GetRotationV(),
					GetMeshRight(), -angleIncrement));
				if (!linkedGridActor->CheckNextRotationBoundsIntersect())
				{
					linkedGridActor->OnLinkRotate();
					linkedGridActor->OnLinkRotateDown();
				}
				else
				{
					linkedGridActor->SetNextRot(linkedGridActor->GetRotationV());
					Camera::GetActiveCamera().SetShakeLevel(0.25f);
				}
			}
		}
	}
	else if (Input::GetKeyUp("RotateUp"))
	{
		if (checkLinkRotation(linkedGridActor->CanBeRotatedInLink()))
		{
			if (linkedGridActor->CanBeRotatedPitchXAxis())
			{
				linkedGridActor->SetNextRot(VMath::AddRotationAngle(linkedGridActor->GetRotationV(),
					GetMeshRight(), angleIncrement));
				if (!linkedGridActor->CheckNextRotationBoundsIntersect())
				{
					linkedGridActor->OnLinkRotate();
					linkedGridActor->OnLinkRotateUp();
				}
				else
				{
					linkedGridActor->SetNextRot(linkedGridActor->GetRotationV());
					Camera::GetActiveCamera().SetShakeLevel(0.25f);
				}
			}
		}
	}
}

void Player::SetDefaultCameraFOV()
{
	nextCameraFOV = 60.f;
}

void Player::SetZoomedInCameraFOV()
{
	nextCameraFOV = 30.f;
}

void Player::CheckNextMoveNode(const XMVECTOR previousPos)
{
	int nextXIndex = (int)std::round(nextPos.m128_f32[0]);
	int nextYIndex = (int)std::round(nextPos.m128_f32[2]);

	//Keep the call here so playerunit can face walls and holes on input.
	_mesh->SetWorldRotation(VMath::LookAtRotation(nextPos, previousPos));
	//Put this here so that quick turns with the mesh with invalid grid node check will still
	//highlight linkable actors. It might mess some stuff up, keep an eye on this bool set.
	previousMovementAndRotationStoppedValue = false;

	auto grid = Grid::system.GetFirstActor();

	if (nextXIndex >= grid->GetSizeX() || nextYIndex >= grid->GetSizeY()
		|| nextXIndex < 0 || nextYIndex < 0)
	{
		nextPos = previousPos;
		return;
	}

	auto nextNode = grid->GetNode(nextXIndex, nextYIndex);

	//Check if active
	if (!nextNode->active)
	{
		Log("Node [x:%d, y:%d] is inactive.", nextXIndex, nextYIndex);
		nextPos = previousPos;
		return;
	}

	//Check next node height in relation to player
	if (nextNode->worldPosition.y > (GetPosition().y + Grid::maxHeightMove))
	{
		Log("Node [x:%d, y:%d] too high to move to.", nextXIndex, nextYIndex);
		nextPos = previousPos;
		return;
	}

	//FENCE RAYCAST CHECK
	HitResult fenceHit(this);
	if (Physics::Raycast(fenceHit, GetPositionV(), nextPos))
	{
		if (dynamic_cast<FenceActor*>(fenceHit.hitActor))
		{
			nextPos = previousPos;
			return;
		}
	}

	CuteHopToLowerNode(nextNode->worldPosition);

	PlayFootstepAudio();

	nextPos = XMLoadFloat3(&nextNode->worldPosition);
}

bool Player::CheckIfMovementAndRotationStopped()
{
	return XMVector4Equal(GetPositionV(), nextPos) && XMQuaternionEqual(GetRotationV(), nextRot);
}

void Player::MovementInput(float)
{
	if (IsInInteraction() || inHop)
	{
		return;
	}

	if (!isInputLinkedToGridActor && CheckIfMovementAndRotationStopped())
	{
		SetGridIndices();

		const XMVECTOR previousPos = nextPos;

		if (Input::GetKeyHeld("MoveForward"))
		{
			nextPos = GetPositionV() + GetForwardVectorV();
			CheckNextMoveNode(previousPos);
		}
		if (Input::GetKeyHeld("MoveBack"))
		{
			nextPos = GetPositionV() + -GetForwardVectorV();
			CheckNextMoveNode(previousPos);
		}
		if (Input::GetKeyHeld("MoveLeft"))
		{
			nextPos = GetPositionV() + -GetRightVectorV();
			CheckNextMoveNode(previousPos);
		}
		if (Input::GetKeyHeld("MoveRight"))
		{
			nextPos = GetPositionV() + GetRightVectorV();
			CheckNextMoveNode(previousPos);
		}
	}
}

void Player::RotationInput(float)
{
	if (IsInInteraction() || inHop)
	{
		return;
	}

	if (!isInputLinkedToGridActor && CheckIfMovementAndRotationStopped())
	{
		if (Input::GetKeyHeld("RotateRight"))
		{
			constexpr float angle = XMConvertToRadians(90.f);
			nextRot = XMQuaternionMultiply(nextRot, DirectX::XMQuaternionRotationAxis(VMath::GlobalUpVector(), angle));
		}
		if (Input::GetKeyHeld("RotateLeft"))
		{
			constexpr float angle = XMConvertToRadians(-90.f);
			nextRot = XMQuaternionMultiply(nextRot, DirectX::XMQuaternionRotationAxis(VMath::GlobalUpVector(), angle));
		}
	}
}

void Player::GetGridIndices(int& x, int& y)
{
	auto pos = GetPosition();
	x = std::lroundf(pos.x);
	y = std::lroundf(pos.z);
}

void Player::SetInteractWidgetText(std::wstring_view interactText)
{
	interactWidget->interactText = interactText;
}

void Player::SetLinkedGridActor(GridActor& gridActor)
{
	nextCameraPosition = cameraLinkActiveLocalPosition;

	ResetHighlightedActor();

	HitResult nodeHit(this);
	gridActor.GetCurrentNode()->RecalcNodeHeight(nodeHit);

	linkedGridActor = &gridActor;

	//Focus on the grid actor the linked grid actor is moving, else focus on the linked actor
	Actor* otherActorToFocusOn = gridActor.GetPlayerFocusActor();
	if (otherActorToFocusOn != nullptr)
	{
		camera->SetTargetActor(otherActorToFocusOn);
	}
	else
	{
		camera->SetTargetActor(&gridActor);
	}

	isInputLinkedToGridActor = true;

	gridActor.OnLinkActivate();

	//Orient player towards the linked actor
	const auto pos = GetPositionV();
	const auto lookAtRotation = VMath::LookAtRotation(pos + GetMeshForward(), pos);
	nextRot = lookAtRotation;
	SetRotation(lookAtRotation);
	_mesh->SetWorldRotation(lookAtRotation);

	EnableLinkEffectMeshForSelect(&gridActor.GetMesh());

	GameUtils::PlayAudioOneShot("confirm.wav");
}

void Player::ResetLinkedGridActor()
{
	isInputLinkedToGridActor = false;

	ResetHighlightedActor();

	if (linkedGridActor)
	{
		linkedGridActor->SetGridPosition();
		HitResult hit(this);
		linkedGridActor->GetCurrentNode()->RecalcNodeHeight(hit);
		linkedGridActor->OnLinkDeactivate();
		linkedGridActor = nullptr;
	}

	if (!CheckIfPlayerIsInsidePlayerCameraTriggerAndReset())
	{
		nextCameraPosition = cameraStartingLocalPosition;
		camera->SetTargetActor(this);
	}

	GameUtils::PlayAudioOneShot("cursor.wav");

	//Check if still looking toward grid actor to keep link effect mesh active.
	const auto playerPos = GetPositionV();
	const auto end = playerPos + (GetMeshForward() * linkDistance);
	HitResult reenableLinkEffectMeshHit(this);
	if (Physics::OrientedBoxCast(reenableLinkEffectMeshHit, playerPos, end, XMFLOAT2(0.25f, 0.25f), false, false))
	{
		auto gridActor = reenableLinkEffectMeshHit.GetClosestHitActorAs<GridActor>(playerPos);
		if (gridActor)
		{
			EnableLinkEffectMeshForHover(&gridActor->GetMesh());
			return;
		}
	}

	linkEffectMesh->SetVisibility(false);
}

void Player::ResetLinkedGridActorIfThis(GridActor* gridActor)
{
	if (gridActor == linkedGridActor)
	{
		linkedGridActor = nullptr;
		isInputLinkedToGridActor = false;
	}

	auto& targetActor = camera->GetTargetActor();
	if (&targetActor == gridActor)
	{
		ResetCameraPosAndTargetToPlayer();
	}
}

void Player::SetNextPosAndRotToCurrent()
{
	nextPos = GetPositionV();
	nextRot = GetRotationV();
}

GridNode* Player::GetCurrentNode()
{
	auto grid = Grid::system.GetFirstActor();
	auto node = grid->GetNode(xIndex, yIndex);
	return node;
}

void Player::SetGridIndices()
{
	xIndex = std::lroundf(GetPosition().x);
	yIndex = std::lroundf(GetPosition().z);
}

void Player::ResetHighlightedActor()
{
	if (highlightedGridActor != nullptr)
	{
		highlightedGridActor->OnPlayerLinkHoverOff();
		highlightedGridActor = nullptr;
	}
}

void Player::OnMoveAndRotateEnd()
{
	const bool stopped = CheckIfMovementAndRotationStopped();
	if (previousMovementAndRotationStoppedValue != stopped)
	{
		InteractInfoToWidgetCheck();
		HighlightLinkableGridActor();
		previousMovementAndRotationStoppedValue = stopped;
	}
}

bool Player::IsInInteraction() const
{
	return inInteraction || inConversation || inInspection;
}

void Player::EnableLinkEffectMeshForHover(MeshComponent* linkMesh)
{
	linkEffectMeshSetAlpha = true;
	linkEffectMesh->SetVisibility(true);

	if (previousHighlightedGridActor != highlightedGridActor)
	{
		linkEffectMesh->SetAlpha(0.f);
	}

	linkEffectMesh->SetUVOffsetSpeed(XMFLOAT2(0.075f, 0.05f));
	linkEffectMesh->SetUVRotationSpeed(0.05f);

	linkEffectMesh->SetWorldScale(linkMesh->GetWorldScaleV() * 1.05f);
	linkEffectMesh->SetWorldPosition(linkMesh->GetWorldPositionV());
	linkEffectMesh->SetWorldRotation(linkMesh->GetWorldRotationV());

	//Make sure the rast state is copied too because grid actors like doors that don't have their backs culled
	//will only show one side of the link effect mesh.
	linkEffectMesh->SetRastState(linkMesh->GetRastState().GetName());

	linkEffectMesh->SetMeshFilename(linkMesh->GetMeshFilename());
	linkEffectMesh->ReCreate();
}

void Player::EnableLinkEffectMeshForSelect(MeshComponent* linkMesh)
{
	linkEffectMeshSetAlpha = true;
	linkEffectMesh->SetVisibility(true);

	linkEffectMesh->SetUVOffsetSpeed(XMFLOAT2(0.15f, 0.1f));
	linkEffectMesh->SetUVRotationSpeed(0.1f);

	linkEffectMesh->SetWorldScale(linkMesh->GetWorldScaleV() * 1.05f);
	linkEffectMesh->SetWorldPosition(linkMesh->GetWorldPositionV());

	linkEffectMesh->SetRastState(linkMesh->GetRastState().GetName());

	linkEffectMesh->SetMeshFilename(linkMesh->GetMeshFilename());
	linkEffectMesh->ReCreate();
}

void Player::UpdateLinkEffectMesh()
{
	if (linkEffectMeshSetAlpha)
	{
		float alpha = linkEffectMesh->GetAlpha();
		alpha += Core::GetDeltaTime() * 2.f;
		if (alpha < 1.f)
		{
			linkEffectMesh->SetAlpha(alpha);
		}
	}
	else
	{
		float alpha = linkEffectMesh->GetAlpha();
		alpha -= Core::GetDeltaTime() * 2.f;
		if (alpha > 0.f)
		{
			linkEffectMesh->SetAlpha(alpha);
		}
		else
		{
			linkEffectMesh->SetVisibility(false);
		}
	}

	if (linkedGridActor)
	{
		auto& linkedMesh = linkedGridActor->GetMesh();
		linkEffectMesh->SetWorldPosition(linkedMesh.GetWorldPositionV());
		linkEffectMesh->SetWorldRotation(linkedMesh.GetWorldRotationV());
	}
}

void Player::PlayFootstepAudio() const
{
	HitResult footStepHit((Actor*)this);
	if (Physics::Raycast(footStepHit, GetPositionV(), -VMath::GlobalUpVector(), 2.f))
	{
		auto meshes = footStepHit.hitActor->GetComponents<MeshComponent>();
		for (auto mesh : meshes)
		{
			auto& material = mesh->GetMaterial();
			const auto audioTypeValue = material.audioTypeValue.GetValue();
			if (audioTypeValue == MaterialAudioType::Default)
			{
				GameUtils::PlayAudioOneShot("step.wav");
			}
		}
	}
}

bool Player::CheckIfMeshCanBeLinkedTo(GridActor* gridActorToLinkTo)
{
	//There should really only ever be one mesh here anyway, but we'll see how it goes.
	for (auto mesh : gridActorToLinkTo->GetComponents<MeshComponent>())
	{
		if (!mesh->canBeLinkedTo)
		{
			Camera::GetActiveCamera().SetShakeLevel(0.3f);
			Log("Cannot link to GridActor via hit mesh [%s]. canBeLinkedTo set to false.", mesh->GetName().c_str());
			return false;
		}
	}

	return true;
}

void Player::CuteHopToLowerNode(const XMFLOAT3 nextNodePos)
{
	const float nextPosYDifference = GetPosition().y - nextNodePos.y;
	constexpr float hopHeight = 0.9f;

	if (nextPosYDifference > hopHeight)
	{
		inHop = true;

		nextHopPos = XMLoadFloat3(&nextNodePos);
		nextHopPos.m128_f32[1] += nextPosYDifference + 0.2f;

		GameUtils::PlayAudioOneShot("jumpland.wav");
	}
}

XMVECTOR Player::GetCameraLocalPosition()
{
	return camera->GetLocalPositionV();
}

void Player::ResetCameraPosAndTargetToPlayer()
{
	nextCameraPosition = cameraStartingLocalPosition;
	camera->SetTargetActor(this);
	camera->SetLerpToFocusPoint(false);
	SetDefaultCameraFOV();
}

void Player::SetCameraTargetActor(Actor* target)
{
	camera->SetTargetActor(target);
}

Actor& Player::GetCameraTargetActor()
{
	return camera->GetTargetActor();
}

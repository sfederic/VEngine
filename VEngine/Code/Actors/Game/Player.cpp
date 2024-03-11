#include "vpch.h"
#include "Player.h"
#include "Core/Camera.h"
#include "Core/Core.h"
#include "Core/Input.h"
#include "Core/VMath.h"
#include "Core/Log.h"
#include "Core/Timer.h"
#include "Physics/Raycast.h"
#include "Actors/Game/NPC.h"
#include "Actors/Game/InspectionTrigger.h"
#include "Actors/Game/FenceActor.h"
#include "Actors/Game/Pickup.h"
#include "Actors/Game/PlayerCameraTrigger.h"
#include "Grid.h"
#include "GridActor.h"
#include "Components/Game/DialogueComponent.h"
#include "Components/MeshComponent.h"
#include "UI/UISystem.h"
#include "UI/Game/DialogueWidget.h"
#include "UI/Game/PickupWidget.h"
#include "UI/Game/InteractWidget.h"
#include "UI/Game/PlayerHealthWidget.h"
#include "Gameplay/GameUtils.h"

Player::Player()
{
	SetEmptyRootComponent();

	camera = CreateComponent<CameraComponent>("Camera");
	rootComponent->AddChild(camera);

	nextPos = XMVectorZero();
	nextRot = XMVectorZero();

	mesh = CreateComponent("Mesh", MeshComponent("char.vmesh", "test.png"));
	rootComponent->AddChild(mesh);

	dialogueComponent = DialogueComponent::system.Add("Dialogue", this);

	camera->SetLocalPosition(cameraStartingLocalPosition);
}

void Player::Create()
{
	__super::Create();

	mesh->SetRenderStatic(false);

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
	healthWidget = UISystem::CreateWidget<PlayerHealthWidget>();
	pickupWidget = UISystem::CreateWidget<PickupWidget>();
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

	OverlapPickupGridActor();

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
	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, moveSpeed));
	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, rotateSpeed));

	//Lerp camera position
	camera->SetLocalPosition(
		VMath::VectorConstantLerp(camera->GetLocalPositionV(), nextCameraPosition, deltaTime, 14.f));
}

Properties Player::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

XMVECTOR Player::GetMeshForward()
{
	return mesh->GetForwardVectorV();
}

XMVECTOR Player::GetMeshRight()
{
	return mesh->GetRightVectorV();
}

bool Player::CheckAttackPositionAgainstUnitDirection(Unit* unit)
{
	if (unit->attackDirections & AttackDirection::All)
	{
		return true;
	}

	auto playerForward = mesh->GetForwardVectorV();
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
	const auto end = playerPos + (GetMeshForward() * 5.f);
	if (OrientedBoxCast(hit, playerPos, end, XMFLOAT2(0.25f, 0.25f), false, false))
	{
		auto hitActor = hit.GetClosestHitActor(playerPos);

		ResetHighlightedActor();

		auto gridActor = dynamic_cast<GridActor*>(hitActor);
		if (gridActor)
		{
			highlightedGridActor = gridActor;

			if (gridActor->IsLinked())
			{
				return;
			}
			if (!gridActor->CanBeMovedInLink() && !gridActor->CanBeRotatedInLink())
			{
				return;
			}

			for (auto mesh : highlightedGridActor->GetComponents<MeshComponent>())
			{
				mesh->SetAmbientColour(XMFLOAT3(0.9f, 0.3f, 0.1f));
			}
		}
		else
		{
			ResetHighlightedActor();
		}
	}
	else
	{
		ResetHighlightedActor();
	}
}

void Player::OverlapPickupGridActor()
{
	if (Input::GetKeyUp(Keys::E))
	{
		const auto center = GetPositionV() + GetMeshForward();
		HitResult hit(this);
		if (SimpleBoxCast(center, XMFLOAT3(0.45f, 0.45f, 0.45f), hit, true, true))
		{
			for (auto hitActor : hit.hitActors)
			{
				auto pickup = dynamic_cast<Pickup*>(hitActor);
				if (pickup)
				{
					interactWidget->interactText = L"Pick up";
					interactWidget->AddToViewport();
				}
			}
		}
		else
		{
			interactWidget->interactText.clear();
			interactWidget->RemoveFromViewport();
		}
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
		auto meshForward = mesh->GetForwardVectorV();
		auto center = GetPositionV();
		auto boxCastOrigin = center + meshForward;
		if (SimpleBoxCast(boxCastOrigin, XMFLOAT3(0.25f, 0.25f, 0.25f), hit, false, true))
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
			currentlyActiveDialogueComponent = npc->dialogueComponent;
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
	if (OrientedBoxCast(hit, start, camera->GetWorldPositionV(), XMFLOAT2(0.25f, 0.25f), true, false))
	{
		for (Actor* actor : hit.hitActors)
		{
			if (actor->CanBeTransparentlyOccluded())
			{
				previousHitTransparentActors.insert(actor);

				auto mesh = actor->GetFirstComponentOfTypeAllowNull<MeshComponent>();
				if (mesh && mesh->transparentOcclude)
				{
					float alpha = mesh->GetAlpha();
					if (alpha > 0.33f)
					{
						alpha -= Core::GetDeltaTime() * 3.f;
						mesh->SetAlpha(alpha);
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
			auto mesh = (*it)->GetFirstComponentOfTypeAllowNull<MeshComponent>();
			if (mesh && mesh->transparentOcclude)
			{
				float alpha = mesh->GetAlpha();
				if (alpha < 1.f)
				{
					alpha += Core::GetDeltaTime() * 1.5f;
					mesh->SetAlpha(alpha);
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

				if (!gridActor->interactText.empty())
				{
					interactWidget->interactText = gridActor->interactText;
					interactWidget->AddToViewport();
				}

				return true;
			}

			if (gridActor->isInteractable)
			{
				gridActor->Interact();
				gridActorInteractingWith = gridActor;

				//@Todo: this is here if GridActor destroys itself on Interact(), but will probably cause visual 'error's
				highlightedGridActor = nullptr;

				return true;
			}
		}
	}

	return false;
}

void Player::InteractInfoToWidgetCheck()
{
	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), GetMeshForward(), 1.f))
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
			if (!gridActor->interactText.empty())
			{
				interactWidget->SetWorldPosition(GetHomogeneousPositionV());
				interactWidget->interactText = L"Inspect";
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
			ResetLinkedGridActor();
		}
	}
	else if (!isInputLinkedToGridActor && Input::GetKeyUp("Link")) //Raycast forward
	{
		HitResult hit(this);
		const auto playerPos = GetPositionV();
		const auto end = playerPos + (GetMeshForward() * 5.f);
		if (OrientedBoxCast(hit, playerPos, end, XMFLOAT2(0.25f, 0.25f), false, false))
		{
			auto closestActorToLinkTo = hit.GetClosestHitActorAs<GridActor>(playerPos);
			if (closestActorToLinkTo == nullptr)
			{
				return;
			}

			//This raycast is to make sure the player is not standing on the same actor it's linking to
			//to avoid potentially rotating the linked actor and the player being stuck in mid-air.
			HitResult sameActorHit(this);
			if (Raycast(sameActorHit, GetPositionV(), -VMath::GlobalUpVector(), 5.f))
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

					for (auto mesh : closestActorToLinkTo->GetComponents<MeshComponent>())
					{
						if (!mesh->canBeLinkedTo)
						{
							Camera::GetActiveCamera().SetShakeLevel(0.3f);
							Log("Cannot link to GridActor via hit mesh [%s]. canBeLinkedTo set to false.", mesh->name.c_str());
							return;
						}
					}
				}

				SetLinkedGridActor(*closestActorToLinkTo, hit.GetHitPosV());
			}
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
				//@Todo: replace this with a shake of the spatial component or some other visual effect
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
					linkedGridActor->OnLinkRotateRight();
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
					linkedGridActor->OnLinkRotateRight();
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
					linkedGridActor->OnLinkRotateLeft();
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
					linkedGridActor->OnLinkRotateLeft();
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
	mesh->SetWorldRotation(VMath::LookAtRotation(nextPos, previousPos));

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

	//Check if drop is too high for player
	const float nextNodeAndPlayerPosHeightDifference = GetPosition().y - nextNode->worldPosition.y;
	if (nextNodeAndPlayerPosHeightDifference > Grid::maxPlayerDropHeight)
	{
		Log("Node [x:%d, y:%d] too low to drop down to.", nextXIndex, nextYIndex);
		nextPos = previousPos;
		return;
	}

	//FENCE RAYCAST CHECK
	HitResult fenceHit(this);
	if (Raycast(fenceHit, GetPositionV(), nextPos))
	{
		if (dynamic_cast<FenceActor*>(fenceHit.hitActor))
		{
			nextPos = previousPos;
			return;
		}
	}

	nextPos = XMLoadFloat3(&nextNode->worldPosition);
}

bool Player::CheckIfMovementAndRotationStopped()
{
	return XMVector4Equal(GetPositionV(), nextPos) && XMQuaternionEqual(GetRotationV(), nextRot);
}

void Player::MovementInput(float deltaTime)
{
	if (IsInInteraction())
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

void Player::RotationInput(float deltaTime)
{
	if (IsInInteraction())
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

void Player::SetLinkedGridActor(GridActor& gridActor, const XMVECTOR playerEyeLevelPos)
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

	//If default value for `playerEyeLevelPos` is used and not set by user, don't set the player's look at rotation.
	if (XMVector4Equal(playerEyeLevelPos, XMVectorZero()))
	{
		Log("Note: playerEyeLevelPos was not set when setting player linked grid actor. Look at rotation won't be set.");
		return;
	}

	//Orient player towards the linked actor's point from the raycast so that input isn't messy from various directions.
	const auto lookAtRotation = VMath::LookAtRotation(playerEyeLevelPos, GetPositionV());
	nextRot = lookAtRotation;
	SetRotation(lookAtRotation);
	mesh->SetWorldRotation(lookAtRotation);
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

void Player::SetPickupWidgetIconFilename(std::string_view filename)
{
	pickupWidget->SetItemIconFilename(filename);
}

void Player::AddPickupWidgetToViewport()
{
	pickupWidget->AddToViewport();
}

void Player::RemovePickupWidgetFromViewport()
{
	pickupWidget->RemoveFromViewport();
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
	if (highlightedGridActor == nullptr)
	{
		return;
	}

	for (auto mesh : highlightedGridActor->GetComponents<MeshComponent>())
	{
		mesh->SetAmbientColour(XMFLOAT3(1.f, 1.f, 1.f));
	}

	highlightedGridActor = nullptr;
}

//@Todo: this function actually fires twice, once when the movement/rotation begins, and once when it ends.
//It's not too bad considering this was written to cut down on the raycasts every tick(), but leaving a light
//todo here to make it work once off might be nicer.
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

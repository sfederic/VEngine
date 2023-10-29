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
}

void Player::Create()
{
	__super::Create();

	cameraStartingLocalPosition = XMVectorSet(1.75f, 1.75f, -2.75f, 1.f);
	cameraLinkActiveLocalPosition = XMVectorSet(1.25f, 0.55f, -0.75f, 1.f);
	camera->SetLocalPosition(cameraStartingLocalPosition);

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

	InteractInfoToWidgetCheck();

	OverlapPickupGridActor();
	HighlightLinkableGridActor();

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

void Player::QuickThought(const std::wstring& text)
{
	dialogueComponent->dialogueWidget->dialogueText = text;
	dialogueComponent->AddToViewport();

	Timer::SetTimer(5.0f, std::bind(&DialogueWidget::RemoveFromViewport, dialogueComponent->dialogueWidget));
}

bool Player::CheckAttackPositionAgainstUnitDirection(Unit* unit)
{
	if (unit->attackDirections == AttackDirection::All)
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
	if (Input::GetKeyUp(Keys::Space))
	{
		GameUtils::PlayAudioOneShot("sword_sheathe.wav");
		auto grid = Grid::system.GetOnlyActor();
		if (grid->lerpValue == Grid::LerpValue::LerpIn)
		{
			grid->lerpValue = Grid::LerpValue::LerpOut;
		}
		else
		{
			grid->lerpValue = Grid::LerpValue::LerpIn;
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
	if (Raycast(hit, GetPositionV(), GetMeshForward(), 100.f))
	{
		ResetHighlightedActor();

		auto gridActor = dynamic_cast<GridActor*>(hit.hitActor);
		if (gridActor)
		{
			if (gridActor->IsLinked())
			{
				return;
			}
			if (!gridActor->CanBeMovedInLink() && !gridActor->CanBeRotatedInLink())
			{
				return;
			}

			highlightedGridActor = gridActor;
			for (auto mesh : highlightedGridActor->GetComponentsOfType<MeshComponent>())
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

	if (Input::GetKeyUp(Keys::Down))
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
			if (gridActorInteractingWith == nullptr) return;

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
			//@Todo: won't work later on for multiple hit actors
			hit.hitActor = hit.hitActors.front();
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
	HitResult hit(this);
	if (OrientedBoxCast(hit, camera->GetWorldPositionV(), GetPositionV(), XMFLOAT2(0.25f, 0.25f), true, false))
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

	std::vector<Actor*> actorsToRemove;

	for (auto it = previousHitTransparentActors.begin(); it != previousHitTransparentActors.end(); it++)
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
				actorsToRemove.push_back(*(it));
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
			interactWidget->worldPosition = GetHomogeneousPositionV();
			interactWidget->interactText = L"Talk";
			interactWidget->AddToViewport();
			return;
		}

		auto gridActor = hit.GetHitActorAs<GridActor>();
		if (gridActor)
		{
			if (!gridActor->interactText.empty())
			{
				interactWidget->worldPosition = GetHomogeneousPositionV();
				interactWidget->interactText = gridActor->interactText;
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
	if (!CheckIfMovementAndRotationStopped())
	{
		return;
	}

	if (isInputLinkedToGridActor) //Cancel
	{
		if (Input::GetKeyUp(Keys::BackSpace))
		{
			ResetLinkedGridActor();
		}
	}
	else if (!isInputLinkedToGridActor && Input::GetKeyUp(Keys::Enter)) //Raycast forward
	{
		HitResult hit(this);
		if (Raycast(hit, GetPositionV(), GetMeshForward(), 100.f))
		{
			//This raycast is to make sure the player is not standing on the same actor it's linking to
			//to avoid potentially rotating the linked actor and the player being stuck in mid-air.
			HitResult sameActorHit(this);
			Raycast(sameActorHit, GetPositionV(), -VMath::GlobalUpVector(), 5.f);
			if (sameActorHit.hitActor == hit.hitActor)
			{
				Camera::GetActiveCamera().SetShakeLevel(0.3f);
				Log("Cannot link to [%s], player is standing on it.", hit.hitActor->GetName().c_str());
				return;
			}

			auto gridActor = dynamic_cast<GridActor*>(hit.hitActor);
			if (gridActor)
			{
				auto hitMesh = dynamic_cast<MeshComponent*>(hit.hitComponent);
				if (hitMesh)
				{
					if (!hitMesh->canBeLinkedTo)
					{
						Log("Cannot link to GridActor via hit mesh [%s]. canBeLinkedTo set to false.", hitMesh->name.c_str());
						return;
					}
				}

				SetLinkedGridActor(*gridActor);
			}
		}
	}
}

void Player::MoveLinkedGridActor()
{
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

	if (Input::GetKeyUp(Keys::Up))
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
	else if (Input::GetKeyUp(Keys::Down))
	{
		if (checkLinkMovement(linkedGridActor->CanBeMovedInLink()))
		{
			if (linkedGridActor->IsNextMoveAxisValid(-GetForwardVectorV()))
			{
				linkedGridActor->AddNextPosition(-GetForwardVectorV());
				if (linkedGridActor->CheckNextNodeMoveIsValid(-GetForwardVectorV()))
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
	else if (Input::GetKeyUp(Keys::Left))
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
	else if (Input::GetKeyUp(Keys::Right))
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

	/*if (Input::GetKeyHeld(Keys::Shift) && Input::GetKeyDown(Keys::Right))
	{
		if (checkLinkRotation(linkedGridActor->CanBeRotatedInLink()))
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
	}
	else if (Input::GetKeyHeld(Keys::Shift) && Input::GetKeyDown(Keys::Left))
	{
		if (checkLinkRotation(linkedGridActor->CanBeRotatedInLink()))
		{
			linkedGridActor->SetNextRot(VMath::AddRotationAngle(linkedGridActor->GetRotationV(),
				VMath::GlobalForwardVector(), angleIncrement));
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
	}*/
	if (Input::GetKeyUp(Keys::NumPad3))
	{
		if (checkLinkRotation(linkedGridActor->CanBeRotatedInLink()))
		{
			if (linkedGridActor->CanBeRotatedYawYAxis())
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
	else if (Input::GetKeyUp(Keys::NumPad1))
	{
		if (checkLinkRotation(linkedGridActor->CanBeRotatedInLink()))
		{
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
	else if (Input::GetKeyUp(Keys::NumPad2))
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
	else if (Input::GetKeyUp(Keys::NumPad5))
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

	if (nextXIndex >= grid->sizeX || nextYIndex >= grid->sizeY
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
	if (inInspection)
	{
		return;
	}

	if (!isInputLinkedToGridActor && CheckIfMovementAndRotationStopped())
	{
		SetGridIndices();

		const XMVECTOR previousPos = nextPos;

		if (Input::GetKeyHeld(Keys::Up))
		{
			nextPos = GetPositionV() + GetForwardVectorV();
			CheckNextMoveNode(previousPos);
		}
		if (Input::GetKeyHeld(Keys::Down))
		{
			nextPos = GetPositionV() + -GetForwardVectorV();
			CheckNextMoveNode(previousPos);
		}
		/*if (Input::GetKeyHeld(Keys::NumPad1))
		{
			nextPos = GetPositionV() + -GetRightVectorV();
			CheckNextMoveNode(previousPos);
		}
		if (Input::GetKeyHeld(Keys::NumPad3))
		{
			nextPos = GetPositionV() + GetRightVectorV();
			CheckNextMoveNode(previousPos);
		}*/
	}
}

void Player::RotationInput(float deltaTime)
{
	if (inInspection)
	{
		return;
	}

	if (!isInputLinkedToGridActor && CheckIfMovementAndRotationStopped())
	{
		if (Input::GetKeyHeld(Keys::Right))
		{
			constexpr float angle = XMConvertToRadians(90.f);
			nextRot = XMQuaternionMultiply(nextRot, DirectX::XMQuaternionRotationAxis(VMath::GlobalUpVector(), angle));
		}
		if (Input::GetKeyHeld(Keys::Left))
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

	//Orient player towards the linked actor so that input isn't messy from various directions.
	auto playerEyeLevelPos = gridActor.GetPositionV();
	//bring the point to player eye level so that rotation is skewed up.
	playerEyeLevelPos.m128_f32[1] = GetPosition().y;
	const auto lookAtRotation = VMath::LookAtRotation(playerEyeLevelPos, GetPositionV());
	nextRot = lookAtRotation;
	SetRotation(lookAtRotation);
	mesh->SetWorldRotation(lookAtRotation);
}

void Player::ResetLinkedGridActor()
{
	nextCameraPosition = cameraStartingLocalPosition;

	ResetHighlightedActor();

	linkedGridActor->SetGridPosition();
	HitResult hit(this);
	linkedGridActor->GetCurrentNode()->RecalcNodeHeight(hit);
	linkedGridActor->OnLinkDeactivate();
	linkedGridActor = nullptr;

	camera->SetTargetActor(this);

	isInputLinkedToGridActor = false;
}

void Player::ResetLinkedGridActorIfThis(GridActor* gridActor)
{
	if (gridActor == linkedGridActor)
	{
		linkedGridActor = nullptr;
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

	for (auto mesh : highlightedGridActor->GetComponentsOfType<MeshComponent>())
	{
		mesh->SetAmbientColour(XMFLOAT3(1.f, 1.f, 1.f));
	}

	highlightedGridActor = nullptr;
}

void Player::ResetCameraPosAndTargetToPlayer()
{
	nextCameraPosition = cameraStartingLocalPosition;
	camera->SetTargetActor(this);
}

void Player::SetCameraTargetActor(Actor* target)
{
	camera->SetTargetActor(target);
}

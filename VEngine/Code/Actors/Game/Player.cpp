#include "vpch.h"
#include "Player.h"
#include "Core/Camera.h"
#include "Core/Input.h"
#include "Core/VMath.h"
#include "Core/Core.h"
#include "Core/Log.h"
#include "Core/Timer.h"
#include "Physics/Raycast.h"
#include "Actors/Game/NPC.h"
#include "Actors/Game/FenceActor.h"
#include "Grid.h"
#include "GridActor.h"
#include "Components/EmptyComponent.h"
#include "Components/AudioComponent.h"
#include "Components/Game/DialogueComponent.h"
#include "UI/UISystem.h"
#include "UI/Game/HealthWidget.h"
#include "UI/Game/DialogueWidget.h"
#include "UI/Game/InteractWidget.h"
#include "UI/Game/PlayerHealthWidget.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "Render/Material.h"
#include "Render/BlendStates.h"

Player::Player()
{
	SetEmptyRootComponent();

	camera = CreateComponent("Camera", CameraComponent(XMFLOAT3(1.75f, 1.75f, -2.75f)));
	rootComponent->AddChild(camera);

	nextPos = XMVectorZero();
	nextRot = XMVectorZero();

	mesh = CreateComponent("Mesh", MeshComponent("char.vmesh", "test.png"));
	rootComponent->AddChild(mesh);

	dialogueComponent = DialogueComponent::system.Add("Dialogue", this);
}

void Player::Create()
{
	moveSpeed = 6.5f;
	rotateSpeed = 6.0f;
}

void Player::Start()
{
	__super::Start();

	nextPos = GetPositionV();
	nextRot = GetRotationV();

	SetGridIndices();

	camera->targetActor = this;
	nextCameraFOV = camera->FOV;
	camera->SetAsActiveCamera();

	//Setup widgets
	interactWidget = UISystem::CreateWidget<InteractWidget>();
	healthWidget = UISystem::CreateWidget<PlayerHealthWidget>();
}

void Player::End()
{
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

	MoveLinkedGridActor();
	RotateLinkedGridActor();
	PushbackGridActor();
	LinkToGridActor();

	MovementInput(deltaTime);
	RotationInput(deltaTime);

	PrimaryAction();

	LerpPlayerCameraFOV(deltaTime);
	MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	dialogueComponent->SetPosition(GetHomogeneousPositionV());

	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, moveSpeed));
	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, rotateSpeed));
}

Properties Player::GetProps()
{
    auto props = Actor::GetProps();
	props.title = "Player";
	return props;
}

XMVECTOR Player::GetMeshForward()
{
	return mesh->GetForwardVectorV();
}

void Player::QuickThought(const std::wstring& text)
{
	dialogueComponent->dialogueWidget->dialogueText = text;
	dialogueComponent->AddToViewport();

	Timer::SetTimer(5.0f, std::bind(&DialogueWidget::RemoveFromViewport, dialogueComponent->dialogueWidget));
}

void Player::ToggleGrid()
{
	if (Input::GetKeyUp(Keys::Space))
	{
		GameUtils::PlayAudioOneShot("sword_sheathe.wav");
		Grid::system.GetOnlyActor()->ToggleActive();
	}
}

void Player::PrimaryAction()
{
	if (Input::GetKeyUp(Keys::Down))
	{
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

		HitResult hit(this);
		auto meshForward = mesh->GetForwardVectorV();
		auto center = GetPositionV();
		auto boxCastOrigin = center + meshForward;
		if (SimpleBoxCast(boxCastOrigin, XMFLOAT3(0.25f, 0.25f, 0.25f), hit, false))
		{
			//@Todo: won't work later on for multiple hit actors
			hit.hitActor = hit.hitActors.front();
			Log("Player interact: %s", hit.hitActor->GetName().c_str());

			if (DialogueCheck(hit.hitActor)) {}
			else if (QuickTalkCheck(hit.hitActor)) {}
			else if (InteractCheck(hit.hitActor)) {}
			else if (DestructibleCheck(hit.hitActor)) {}
		}
	}
}

void Player::LerpPlayerCameraFOV(float deltaTime)
{
	if (camera->FOV != nextCameraFOV)
	{
		camera->FOV = std::lerp(camera->FOV, nextCameraFOV, 4.f * deltaTime);
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
			GameUtils::SetActiveCameraTarget(this);
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
	auto SetActorAlpha = [](Actor* actor, float alpha) {
		auto mesh = actor->GetFirstComponentOfTypeAllowNull<MeshComponent>();
		if (mesh && mesh->transparentOcclude)
		{
			auto ambientColour = mesh->GetAmbientColour();
			ambientColour.w = alpha;
			mesh->SetAmbientColour(ambientColour);
		}
	};

	const float transparentValue = 0.35f;
	const float solidValue = 1.f;

	HitResult hit(this);
	if (OrientedBoxCast(hit, camera->GetWorldPositionV(), GetPositionV(), XMFLOAT2(0.5f, 0.5f), true))
	{
		std::vector<Actor*> ableActors;

		for (auto actor : previousHitTransparentActors)
		{
			SetActorAlpha(actor, solidValue);
		}

		for (auto actor : hit.hitActors)
		{
			if (actor->CanBeTransparentlyOccluded())
			{
				ableActors.push_back(actor);
				SetActorAlpha(actor, transparentValue);
			}
		}

		previousHitTransparentActors = ableActors;
	}
	else
	{
		for(auto actor : previousHitTransparentActors)
		{
			SetActorAlpha(actor, solidValue);
		}

		previousHitTransparentActors.clear();
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
	if (!inConversation)
	{
		auto gridActor = dynamic_cast<GridActor*>(hitActor);
		if (gridActor)
		{
			if (gridActor->isInteractable)
			{
				gridActor->Interact();
				gridActorInteractingWith = gridActor;
			}

			if (gridActor->isInspectable)
			{
				interactWidget->interactText = gridActor->interactText;
				interactWidget->AddToViewport();
				inInteraction = true;

				SetZoomedInCameraFOV();

				return true;
			}
		}
	}

	return false;
}

bool Player::DestructibleCheck(Actor* hitActor)
{
	if (!inConversation)
	{
		auto npc = dynamic_cast<NPC*>(hitActor);
		if (npc)
		{
			npc->EndQuickTalkTo();
		}

		auto unit = dynamic_cast<Unit*>(hitActor);
		if (unit)
		{
			if (CheckAttackPositionAgainstUnitDirection(unit))
			{
				GameUtils::CameraShake(1.f);
				GameUtils::SpawnSpriteSheet("Sprites/v_slice.png", unit->GetPositionV(), false, 4, 4);
				GameUtils::PlayAudioOneShot("sword_hit.wav");

				unit->InflictDamage(1);
			}
			else
			{
				Log("Attack positional miss on Unit [%s].", unit->GetName().c_str());
			}

			return true;
		}

		auto gridActor = dynamic_cast<GridActor*>(hitActor);
		if (gridActor)
		{
			GameUtils::CameraShake(1.f);
			GameUtils::SpawnSpriteSheet("Sprites/v_slice.png", gridActor->GetPositionV(), false, 4, 4);
			GameUtils::PlayAudioOneShot("sword_hit.wav");

			gridActor->InflictDamage(1);

			return true;
		}
	}

	return false;
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

void Player::LinkToGridActor()
{
	if (!isInputLinkedToGridActor && Input::GetKeyUp(Keys::Up))
	{
		HitResult hit(this);
		if (Raycast(hit, GetPositionV(), GetForwardVectorV(), 100.f))
		{
			auto moveableActor = dynamic_cast<GridActor*>(hit.hitActor);
			if (moveableActor)
			{ 
				HitResult hit(moveableActor);
				moveableActor->GetCurrentNode()->RecalcNodeHeight(hit);
				linkedGridActor = moveableActor;
				camera->targetActor = moveableActor;
				isInputLinkedToGridActor = true;
			}
		}
	}
	else if (isInputLinkedToGridActor)
	{
		if (Input::GetKeyUp(Keys::BackSpace))
		{
			HitResult hit(linkedGridActor);
			linkedGridActor->GetCurrentNode()->RecalcNodeHeight(hit);
			linkedGridActor = nullptr;
			camera->targetActor = this;
			isInputLinkedToGridActor = false;
			return;
		}
	}
}

void Player::MoveLinkedGridActor()
{
	if (linkedGridActor == nullptr || !linkedGridActor->HaveMovementAndRotationStopped())
	{
		return;
	}

	const auto checkLinkMovement = [](bool canBeMoved) -> bool
	{
		if (!canBeMoved)
		{
			//@Todo: replace this with a shake of the spatial component instead
			GameUtils::CameraShake(0.25f);
			return false;
		}
		return true;
	};

	if (Input::GetKeyUp(Keys::W))
	{
		if (checkLinkMovement(linkedGridActor->canBeMovedInLink))
		{
			linkedGridActor->nextPos += GetForwardVectorV();
			linkedGridActor->CheckNextNodeMoveIsValid();
		}
	}
	else if (Input::GetKeyUp(Keys::S))
	{
		if (checkLinkMovement(linkedGridActor->canBeMovedInLink))
		{
			linkedGridActor->nextPos -= GetForwardVectorV();
			linkedGridActor->CheckNextNodeMoveIsValid();
		}
	}
	else if (Input::GetKeyUp(Keys::A))
	{
		if (checkLinkMovement(linkedGridActor->canBeMovedInLink))
		{
			linkedGridActor->nextPos -= GetRightVectorV();
			linkedGridActor->CheckNextNodeMoveIsValid();
		}
	}
	else if (Input::GetKeyUp(Keys::D))
	{
		if (checkLinkMovement(linkedGridActor->canBeMovedInLink))
		{
			linkedGridActor->nextPos += GetRightVectorV();
			linkedGridActor->CheckNextNodeMoveIsValid();
		}
	}
	else if (Input::GetKeyUp(Keys::E))
	{
		if (checkLinkMovement(linkedGridActor->canBeMovedInLink))
		{
			linkedGridActor->nextPos += GetUpVectorV();
			linkedGridActor->CheckNextNodeMoveIsValid();
		}
	}
	else if (Input::GetKeyUp(Keys::Q))
	{
		if (checkLinkMovement(linkedGridActor->canBeMovedInLink))
		{
			linkedGridActor->nextPos -= GetUpVectorV();
			linkedGridActor->CheckNextNodeMoveIsValid();
		}
	}
}

void Player::RotateLinkedGridActor()
{
	if (linkedGridActor == nullptr || !linkedGridActor->HaveMovementAndRotationStopped())
	{
		return;
	}

	const auto checkLinkRotation = [](bool canBeRotated) -> bool
	{
		if (!canBeRotated)
		{
			GameUtils::CameraShake(0.25f);
			return false;
		}
		return true;
	};

	if (Input::GetKeyUp(Keys::Right))
	{
		if (checkLinkRotation(linkedGridActor->canBeRotatedInLink))
		{
			linkedGridActor->nextRot = VMath::AddRotationAngle(linkedGridActor->GetRotationV(),
				VMath::GlobalUpVector(), 90.f);
		}
	}
	else if (Input::GetKeyUp(Keys::Left))
	{
		if (checkLinkRotation(linkedGridActor->canBeRotatedInLink))
		{
			linkedGridActor->nextRot = VMath::AddRotationAngle(linkedGridActor->GetRotationV(),
				VMath::GlobalUpVector(), -90.f);
		}
	}
	else if (Input::GetKeyUp(Keys::Down))
	{
		if (checkLinkRotation(linkedGridActor->canBeRotatedInLink))
		{
			linkedGridActor->nextRot = VMath::AddRotationAngle(linkedGridActor->GetRotationV(),
				VMath::GlobalRightVector(), -90.f);
		}
	}
	else if (Input::GetKeyUp(Keys::Up))
	{
		if (checkLinkRotation(linkedGridActor->canBeRotatedInLink))
		{
			linkedGridActor->nextRot = VMath::AddRotationAngle(linkedGridActor->GetRotationV(),
				VMath::GlobalRightVector(), 90.f);
		}
	}
}

void Player::PushbackGridActor()
{
	if (Input::GetKeyUp(Keys::X))
	{
		HitResult hit(this);
		if (Raycast(hit, GetPositionV(), GetForwardVectorV(), 10.f))
		{
			auto gridActor = dynamic_cast<GridActor*>(hit.hitActor);
			if (gridActor)
			{
				gridActor->Pushback(GetForwardVectorV());
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

void Player::CheckNextMoveNode(XMVECTOR previousPos)
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

	auto nextNodeToMoveTo = grid->GetNode(nextXIndex, nextYIndex);
	if (!nextNodeToMoveTo->active)
	{
		nextPos = previousPos;
		return;
	}

	//Check next node height in relation to player
	auto node = grid->GetNode(nextXIndex, nextYIndex);
	if (node->worldPosition.y > (GetPosition().y + Grid::maxHeightMove))
	{
		Log("Node [x:%d, y:%d] too high to move to.", nextXIndex, nextYIndex);
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

	nextPos = XMLoadFloat3(&node->worldPosition);
}

bool Player::CheckIfMovementAndRotationStopped()
{
	return XMVector4Equal(GetPositionV(), nextPos) && XMQuaternionEqual(GetRotationV(), nextRot);
}

void Player::MovementInput(float deltaTime)
{
	if (!isInputLinkedToGridActor && CheckIfMovementAndRotationStopped())
	{
		SetGridIndices();

		XMVECTOR previousPos = nextPos;

		if (Input::GetKeyHeld(Keys::W))
		{
			nextPos = GetPositionV() + GetForwardVectorV();
			CheckNextMoveNode(previousPos);
		}

		if (Input::GetKeyHeld(Keys::S))
		{
			nextPos = GetPositionV() + -GetForwardVectorV();
			CheckNextMoveNode(previousPos);
		}
		if (Input::GetKeyHeld(Keys::A))
		{
			nextPos = GetPositionV() + -GetRightVectorV();
			CheckNextMoveNode(previousPos);
		}
		if (Input::GetKeyHeld(Keys::D))
		{
			nextPos = GetPositionV() + GetRightVectorV();
			CheckNextMoveNode(previousPos);
		}
	}
}

void Player::RotationInput(float deltaTime)
{
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

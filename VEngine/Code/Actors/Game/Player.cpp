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
#include "Actors/Game/GridMapPicker.h"
#include "Actors/Game/MemoryCheckGridActor.h"
#include "Grid.h"
#include "GridActor.h"
#include "Components/EmptyComponent.h"
#include "Components/Game/MemoryComponent.h"
#include "Components/AudioComponent.h"
#include "Components/Game/DialogueComponent.h"
#include "UI/UISystem.h"
#include "UI/Game/HealthWidget.h"
#include "UI/Game/DialogueWidget.h"
#include "UI/Game/InteractWidget.h"
#include "UI/Game/MemoryMenuWidget.h"
#include "UI/Game/PlayerActionBarWidget.h"
#include "UI/Game/MemoryGainedWidget.h"
#include "UI/Game/MemoryRecalledWidget.h"
#include "UI/Game/GuardWidget.h"
#include "UI/Game/PlayerHealthWidget.h"
#include "UI/Game/PlayerStatusWidget.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/BattleSystem.h"
#include "Gameplay/Trap.h"
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
	memoryMenuWidget = UISystem::CreateWidget<MemoryMenuWidget>();
	healthWidget = UISystem::CreateWidget<PlayerHealthWidget>();
	guardWidget = UISystem::CreateWidget<GuardWidget>();
	playerStatusWidget = UISystem::CreateWidget<PlayerStatusWidget>();

	battleSystem.actionBarWidget = UISystem::CreateWidget<PlayerActionBarWidget>();
	battleSystem.actionBarWidget->actionPoints = battleSystem.playerActionPoints;
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

	MovementInput(deltaTime);
	RotationInput(deltaTime);

	PrimaryAction();
	EnterAstralMode();
	ToggleMemoryMenu();

	LerpPlayerCameraFOV(deltaTime);
	MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	dialogueComponent->SetPosition(GetHomogeneousPositionV());
	playerStatusWidget->worldPosition = GetHomogeneousPositionV();

	if (battleSystem.isBattleActive)
	{
		battleSystem.actionBarWidget->AddToViewport();
	}
	else
	{
		battleSystem.actionBarWidget->RemoveFromViewport();
	}

	if (!inConversation && !inInteraction)
	{
		//Skip movement if not player's turn during combat
		if (battleSystem.isBattleActive && !battleSystem.isPlayerTurn)
		{
			return;
		}

		if (battleSystem.isBattleActive && battleSystem.playerActionPoints < 0)
		{
			return;
		}
	}

	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, moveSpeed));
	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, rotateSpeed));
}

Properties Player::GetProps()
{
    auto props = Actor::GetProps();
	props.title = "Player";
	return props;
}

void Player::RefreshCombatStats()
{
	ResetGuard();

	if (isFatigued) 
	{
		battleSystem.playerActionPoints = GameInstance::maxPlayerActionPoints / 2;

		isFatigued = false;
		playerStatusWidget->RemoveFromViewport();
	} 
	else 
	{
		battleSystem.playerActionPoints = GameInstance::maxPlayerActionPoints;
	}

	battleSystem.actionBarWidget->actionPoints = battleSystem.playerActionPoints;
}

void Player::BattleCleanup()
{
	inBattleMode = false;
	battleSystem.isPlayerTurn = false;

	RefreshCombatStats();

	healthWidget->RemoveFromViewport();

	GameUtils::SetActiveCamera(camera);
	GameUtils::SetActiveCameraTarget(this);
}

void Player::SetupForBattle()
{
	battleSystem.isPlayerTurn = true;
	inBattleMode = true;

	healthWidget->AddToViewport();
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

void Player::EnterAstralMode()
{
	if (Input::GetKeyUp(Keys::Space))
	{
		inBattleMode = !inBattleMode;

		if (inBattleMode)
		{
			GameUtils::PlayAudioOneShot("sword_hit.wav");
			healthWidget->AddToViewport();
			//battleSystem.StartBattle();
		}
		else
		{
			GameUtils::PlayAudioOneShot("sword_sheathe.wav");
			healthWidget->RemoveFromViewport();
		}

		//toggle grid
		auto grid = Grid::system.GetFirstActor();
		if (grid)
		{
			switch (inBattleMode)
			{
			case true:
				grid->lerpValue = Grid::LerpValue::LerpOut;
				break;
			case false:
				grid->lerpValue = Grid::LerpValue::LerpIn;
				break;
			}

			if (grid->lerpValue == Grid::LerpValue::LerpOut)
			{
				grid->SetActive(true);
			}
		}

		//toggle all Unit health widgets
		auto healthWidgets = UISystem::GetAllWidgetsOfType<HealthWidget>();
		for (auto healthWidget : healthWidgets)
		{
			if (inBattleMode)
			{
				healthWidget->AddToViewport();
			}
			else
			{
				healthWidget->RemoveFromViewport();
			}
		}
	}
}

void Player::PrimaryAction()
{
	//End turn input
	if (battleSystem.isPlayerTurn)
	{
		if (battleSystem.isBattleActive && Input::GetKeyUp(Keys::Enter))
		{
			battleSystem.MoveToNextTurn();
			return;
		}
	}

	if (Input::GetKeyUp(Keys::Down))
	{
		//Guard
		if (battleSystem.isBattleActive && !battleSystem.isPlayerTurn)
		{
			if (ableToGuard)
			{
				Guard();
			}

			return;
		}

		/*if (UISystem::memoryRecalledWidget->IsInViewport() 
			|| UISystem::memoryGainedWidget->IsInViewport())
		{
			UISystem::memoryGainedWidget->RemoveFromViewport();
			UISystem::memoryRecalledWidget->RemoveFromViewport();
			return;
		}*/

		if (inInteraction)
		{
			//End interact with GridActor
			if (gridActorInteractingWith == nullptr) return;
			gridActorInteractingWith->EndInteract();
			gridActorInteractingWith = nullptr;

			interactWidget->RemoveFromViewport();
			inInteraction = false;
			nextCameraFOV = 60.f;
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
		else
		{
			//@Todo: was causing weird raycast issues. Come back to this for smaller enemies and whatever else.
			//if (!AttackGridActorBasedOnNode())
			{
				if (inBattleMode)
				{
					GameUtils::PlayAudioOneShot("sword_miss.wav");
				}
			}
		}
	}
}

void Player::ToggleMemoryMenu()
{
	if (inBattleMode || battleSystem.isBattleActive) return;

	if (Input::GetKeyUp(Keys::Enter))
	{
		memoryWidgetToggle = !memoryWidgetToggle;

		if (memoryWidgetToggle)
		{
			//Toggling off memory widget is handled in its own class because it pauses the game world.
			Core::gameWorldPaused = true;
			memoryMenuWidget->AddToViewport();
			GameUtils::PlayAudioOneShot("confirm.wav");
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
			nextCameraFOV = 60.f;
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

			nextCameraFOV = 30.f;

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
	if (!inBattleMode && !inConversation)
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
	if (!inBattleMode && !inConversation)
	{
		auto gridActor = dynamic_cast<GridActor*>(hitActor);
		if (gridActor)
		{
			auto memoryCheckGridActor = dynamic_cast<MemoryCheckGridActor*>(hitActor);
			if (memoryCheckGridActor)
			{
				memoryCheckGridActor->CheckMemory();
			}

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

				nextCameraFOV = 30.f;

				auto memory = gridActor->memoryComponent;
				if (memory->addOnInteract)
				{
					if (!memory->CreateMemory(gridActor->GetName()))
					{
						interactWidget->interactText = gridActor->interactKnownText;
					}
				}

				return true;
			}
		}
	}

	return false;
}

bool Player::DestructibleCheck(Actor* hitActor)
{
	if (inBattleMode && !inConversation)
	{
		auto npc = dynamic_cast<NPC*>(hitActor);
		if (npc)
		{
			npc->EndQuickTalkTo();
		}

		auto unit = dynamic_cast<Unit*>(hitActor);
		if (unit)
		{
			battleSystem.StartBattle();

			if (CheckAttackPositionAgainstUnitDirection(unit))
			{
				ExpendActionPoint();
				GameUtils::CameraShake(1.f);
				GameUtils::SpawnSpriteSheet("Sprites/v_slice.png", unit->GetPositionV(), false, 4, 4);
				GameUtils::PlayAudioOneShot("sword_hit.wav");

				unit->InflictDamage(attackPoints);
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

			gridActor->InflictDamage(attackPoints);

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
			gridActor->InflictDamage(attackPoints);
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

void Player::PlaceTrap(Trap* trap)
{
	auto currentNode = GetCurrentNode();
	if (currentNode->trap == nullptr)
	{
		currentNode->trap = trap;
		currentNode->trap->connectedNode = currentNode;
		currentNode->SetColour(GridNode::trapNodeColour);
	}
}

void Player::Guard()
{
	if (battleSystem.playerActionPoints > 0 && !guarding)
	{
		guarding = true;
		ExpendActionPoint();
		guardWidget->SetGuardSuccess();
		GameUtils::PlayAudioOneShot("equip.wav");
	}
}

void Player::ResetGuard()
{
	guarding = false;
	guardWidget->ResetGuard();
	guardWidget->RemoveFromViewport();
	ableToGuard = true;
}

void Player::SetNormalCameraFOV()
{
	nextCameraFOV = 60.f;
}

void Player::SetZoomedInCameraFOV()
{
	nextCameraFOV = 30.f;
}

void Player::SetGuard()
{
	guardWidget->AddToViewport();
	ableToGuard = true;
}

void Player::CheckNextMoveNode(XMVECTOR previousPos)
{
	if (battleSystem.playerActionPoints <= 0)
	{
		Log("No Player action points remaining.");
		nextPos = previousPos;
		return;
	}

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

	if (battleSystem.isBattleActive)
	{
		ExpendActionPoint();
	}
}

bool Player::CheckIfMovementAndRotationStopped()
{
	return XMVector4Equal(GetPositionV(), nextPos) && XMQuaternionEqual(GetRotationV(), nextRot);
}

void Player::MovementInput(float deltaTime)
{
	if (CheckIfMovementAndRotationStopped())
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
	if (CheckIfMovementAndRotationStopped())
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

void Player::ToggleGridMapPicker(bool& gridPickerActive)
{
	if (Input::GetKeyUp(Keys::I))
	{
		gridPickerActive = !gridPickerActive;

		if (gridPickerActive)
		{
			Transform transform;
			transform.position = GetPosition();
			transform.rotation = GetRotation();
			auto gridMapPicker = GridMapPicker::system.Add(transform);

			gridMapPicker->camera->targetActor = gridMapPicker;
			GameUtils::SetActiveCamera(gridMapPicker->camera);

			Grid::system.GetFirstActor()->SetActive(true);

			SetTickEnabled(false);
		}
		else
		{
			GridMapPicker::system.GetFirstActor()->ReenablePlayer();
		}
	}
}

void Player::ExpendActionPoint()
{
	battleSystem.playerActionPoints--;
	battleSystem.actionBarWidget->actionPoints = battleSystem.playerActionPoints;

	if (battleSystem.playerActionPoints <= 0)
	{
		//Enter fatigue state
		isFatigued = true;
		playerStatusWidget->AddToViewport();
	}
}

void Player::InflictDamage(int damage)
{
	if (guarding)
	{
		guarding = false;
		return;
	}

	healthPoints -= damage;
	if (healthPoints <= 0)
	{
		//@Todo: trigger game over
	}
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

#include "Player.h"
#include "Components/MeshComponent.h"
#include "Components/EmptyComponent.h"
#include "Components/MemoryComponent.h"
#include "Components/AudioComponent.h"
#include "Camera.h"
#include "Input.h"
#include "VMath.h"
#include "VString.h"
#include "Physics/Raycast.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/NPC.h"
#include "Actors/Game/FenceActor.h"
#include "Components/DialogueComponent.h"
#include "Grid.h"
#include "GridActor.h"
#include "UI/HealthWidget.h"
#include "UI/DialogueWidget.h"
#include "UI/InteractWidget.h"
#include "UI/MemoryMenuWidget.h"
#include "UI/PlayerActionBarWidget.h"
#include "UI/MemoryGainedWidget.h"
#include "UI/MemoryRecalledWidget.h"
#include "UI/TimeOfDayWidget.h"
#include "UI/GuardWidget.h"
#include "UI/PlayerHealthWidget.h"
#include "Gameplay/GameInstance.h"
#include "Log.h"
#include "Timer.h"
#include "Gameplay/BattleSystem.h"
#include "Gameplay/Memory.h"

Player::Player()
{
	nextPos = XMVectorZero();
	nextRot = XMVectorZero();

	//Empty as the root to be able to rotate the mesh towards movement input direction.
	rootComponent = EmptyComponent::system.Add(this);

	mesh = MeshComponent::system.Add(this, MeshComponent("character_test.fbx", "test.png"));
	rootComponent->AddChild(mesh);

	weaponMesh = MeshComponent::system.Add(this);
	weaponMesh->active = false;
	weaponMesh->SetPosition(0.5f, 0.f, 0.f); //Local offset
	rootComponent->AddChild(weaponMesh);

	camera = CameraComponent::system.Add(this, CameraComponent(XMFLOAT3(1.5f, 1.5f, -2.5f), false));

	rootComponent->AddChild(camera);

	dialogueComponent = DialogueComponent::system.Add(this);

	stepSounds = AudioComponent::system.Add(this);
	stepSounds->loop = true;
	stepSounds->audioFilename = "step.wav";
	stepSounds->playOnStart = true;
}

void Player::Start()
{
	camera->targetActor = this;

	//Setup widgets
	interactWidget = CreateWidget<InteractWidget>();
	memoryMenuWidget = CreateWidget<MemoryMenuWidget>();

	actionBarWidget = CreateWidget<PlayerActionBarWidget>();
	actionBarWidget->actionPoints = actionPoints;

	CreateWidget<TimeOfDayWidget>()->AddToViewport();

	guardWidget = CreateWidget<GuardWidget>();

	healthWidget = CreateWidget<PlayerHealthWidget>();
	healthWidget->healthPoints = healthPoints;

	nextPos = GetPositionVector();
	nextRot = GetRotationVector();
	nextCameraFOV = camera->FOV;

	xIndex = std::round(GetPosition().x);
	yIndex = std::round(GetPosition().z);
}

void Player::Tick(float deltaTime)
{
	if (gameOver)
	{
		return;
	}

	if (healthPoints <= 0)
	{
		GameUtils::TriggerGameOver();
	}

	//Show gun range tiles
	if (gunModeOn && CheckIfPlayerMovementAndRotationStopped())
	{
		auto meshForward = GetMeshForward();
		auto grid = GameUtils::GetGrid();
		auto currentNode = GetCurrentNode();

		for (int i = 0; i < 5; i++)
		{
			int nextXIndex = xIndex + (i * meshForward.m128_f32[0]);
			int nextYIndex = yIndex + (i * meshForward.m128_f32[2]);

			if (nextXIndex >= grid->sizeX || nextXIndex < 0) break;
			if (nextYIndex >= grid->sizeY || nextYIndex < 0) break;

			auto nextNode = grid->GetNode(nextXIndex, nextYIndex);
			nextNode->SetColour(GridNode::previewColour);
		}
	}

	ToggleBattleGrid();
	ToggleMemoryMenu();

	PrimaryAction();
	SecondaryAction();

	SpawnMemoryAsObject();

	LerpPlayerCameraFOV(deltaTime);

	dialogueComponent->SetPosition(GetHomogeneousPositionVector());

	if (battleSystem.isBattleActive)
	{
		actionBarWidget->AddToViewport();
	}
	else
	{
		actionBarWidget->RemoveFromViewport();
	}

	if (!inConversation && !inInteraction)
	{
		//Skip movement if not player's turn during combat
		if (battleSystem.isBattleActive && !isPlayerTurn)
		{
			return;
		}

		if (battleSystem.isBattleActive && actionPoints < 0)
		{
			return;
		}

		MovementInput(deltaTime);
		RotationInput(deltaTime);
	}
}

Properties Player::GetProps()
{
    auto props = Actor::GetProps();
	props.title = "Player";
	return props;
}

void Player::RefreshCombatStats()
{
	actionPoints = GameInstance::maxPlayerActionPoints;
	actionBarWidget->actionPoints = actionPoints;
}

GridNode* Player::GetCurrentNode()
{
	auto grid = GameUtils::GetGrid();
	auto node = grid->GetNode(xIndex, yIndex);
	return node;
}

void Player::InflictDamage(int damage)
{
	int guardPoints = 0;

	if (guarding)
	{
		Log("Guarded attack");
		guarding = false;
		return;
	}

	healthPoints -= damage;
	healthWidget->healthPoints = healthPoints;

	if (healthPoints <= 0)
	{
		Log("Game Over");
	}
}

void Player::Guard()
{
	if (Input::GetKeyUp(Keys::Down) && actionPoints > 0)
	{
		guarding = true;
		guardWidget->guardSuccessful = true;

		ExpendActionPoints(1);

		GameUtils::PlayAudioOneShot("equip.wav");
	}
}

void Player::BattleCleanup()
{
	isWeaponDrawn = false;
	isPlayerTurn = false;

	RefreshCombatStats();

	healthWidget->RemoveFromViewport();
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

void Player::MovementInput(float deltaTime)
{
	float moveSpeed = 4.75f;
	if (battleSystem.isBattleActive)
	{
		moveSpeed = 6.5f;
	}

	SetPosition(VMath::VectorConstantLerp(GetPositionVector(), nextPos, deltaTime, moveSpeed));

	if (XMVector4Equal(GetPositionVector(), nextPos) && XMQuaternionEqual(GetRotationVector(), nextRot))
	{
		if (stepSoundsVolume > 0.f) { stepSoundsVolume -= deltaTime * 4.f; }
		stepSounds->volume = stepSoundsVolume;

		xIndex = std::round(GetPosition().x);
		yIndex = std::round(GetPosition().z);

		XMVECTOR previousPos = nextPos;

		if (Input::GetAsyncKey(Keys::W))
		{
			nextPos = GetPositionVector() + GetForwardVectorV();
			CheckNextMoveNode(previousPos);
		}
		if (Input::GetAsyncKey(Keys::S))
		{
			nextPos = GetPositionVector() + -GetForwardVectorV();
			CheckNextMoveNode(previousPos);
		}
		if (Input::GetAsyncKey(Keys::A))
		{
			nextPos = GetPositionVector() + -GetRightVectorV();
			CheckNextMoveNode(previousPos);
		}
		if (Input::GetAsyncKey(Keys::D))
		{
			nextPos = GetPositionVector() + GetRightVectorV();
			CheckNextMoveNode(previousPos);
		}

		//Old raycast check for movement blocking (current check uses grid nodes)
		/*if (!XMVector4Equal(previousPos, nextPos))
		{
			Ray ray(this);
			XMVECTOR direction = XMVector3Normalize(nextPos - previousPos);
			if (Raycast(ray, GetPositionVector(), direction, 1.f))
			{
				nextPos = previousPos;
			}
		}*/
	}
	else
	{
		if (stepSoundsVolume < 1.0f) { stepSoundsVolume += deltaTime * 4.f; }
		stepSounds->volume = stepSoundsVolume;
	}
}

void Player::RotationInput(float deltaTime)
{
	float rotSpeed = 5.0f;
	if (battleSystem.isBattleActive)
	{
		rotSpeed = 6.0f;
	}

	SetRotation(VMath::QuatConstantLerp(GetRotationVector(), nextRot, deltaTime, rotSpeed));

	if (XMQuaternionEqual(GetRotationVector(), nextRot) && XMVector4Equal(GetPositionVector(), nextPos))
	{
		if (Input::GetKeyUp(Keys::Right))
		{
			constexpr float angle = XMConvertToRadians(90.f);
			nextRot = XMQuaternionMultiply(nextRot, DirectX::XMQuaternionRotationAxis(VMath::XMVectorUp(), angle));
		}
		if (Input::GetKeyUp(Keys::Left))
		{
			constexpr float angle = XMConvertToRadians(-90.f);
			nextRot = XMQuaternionMultiply(nextRot, DirectX::XMQuaternionRotationAxis(VMath::XMVectorUp(), angle));
		}
	}
}

void Player::ToggleBattleGrid()
{
	if (Input::GetKeyUp(Keys::Space))
	{
		//switch weapons if battle is active
		if (battleSystem.isBattleActive)
		{
			gunModeOn = !gunModeOn;
			return;
		}

		isWeaponDrawn = !isWeaponDrawn;

		//toggle grid
		auto grid = GameUtils::GetGrid();
		if (grid)
		{
			switch (isWeaponDrawn)
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

		if (isWeaponDrawn)
		{
			GameUtils::PlayAudioOneShot("sword_hit.wav");
			healthWidget->AddToViewport();
		}
		else
		{
			GameUtils::PlayAudioOneShot("sword_sheathe.wav");
			healthWidget->RemoveFromViewport();
		}

		//toggle all Unit health widgets
		auto healthWidgets = uiSystem.GetAllWidgetsOfType<HealthWidget>();
		for (auto healthWidget : healthWidgets)
		{
			if (isWeaponDrawn)
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
	if (isPlayerTurn)
	{
		if (battleSystem.isBattleActive && Input::GetKeyUp(Keys::Enter))
		{
			battleSystem.MoveToNextTurn();
			return;
		}
	}

	//Guard
	if (battleSystem.isBattleActive && !isPlayerTurn)
	{
		if (ableToGuard)
		{
			Guard();
		}

		return;
	}

	if (Input::GetKeyUp(Keys::Down))
	{
		if (uiSystem.memoryWidgetInViewport)
		{
			uiSystem.memoryGainedWidget->RemoveFromViewport();
			uiSystem.memoryRecalledWidget->RemoveFromViewport();
			uiSystem.memoryWidgetInViewport = false;
			return;
		}

		if (inInteraction)
		{
			interactWidget->RemoveFromViewport();
			inInteraction = false;
			nextCameraFOV = 60.f;
			return;
		}

		Ray ray(this);
		auto meshForward = mesh->GetForwardVectorV();
		if (Raycast(ray, GetPositionVector(), meshForward, 1.5f))
		{
			Log("Player interact: %s", ray.hitActor->name.c_str());

			if (DestructibleCheck(ray.hitActor)) { return; }
			if (DialogueCheck(ray.hitActor)) {}
			else if (QuickTalkCheck(ray.hitActor)) {}
			else if (InteractCheck(ray.hitActor)) {}
		}
		else
		{
			if (isWeaponDrawn)
			{
				GameUtils::PlayAudioOneShot("sword_miss.wav");
			}
		}
	}
}

//Using this for secondary action testing where primary doesn't fit.
void Player::SecondaryAction()
{
	if (Input::GetKeyUp(Keys::Up))
	{
		Ray ray(this);
		auto meshForward = mesh->GetForwardVectorV();
		if (Raycast(ray, GetPositionVector(), meshForward, 1.5f))
		{
			Log("Player interact: %s", ray.hitActor->name.c_str());

			if (CombatInteractCheck(ray.hitActor)) {}
		}
	}
}

void Player::ToggleMemoryMenu()
{
	if (isWeaponDrawn || battleSystem.isBattleActive) return;

	if (Input::GetKeyUp(Keys::Enter))
	{
		memoryWidgetToggle = !memoryWidgetToggle;

		if (memoryWidgetToggle)
		{
			memoryMenuWidget->AddToViewport();
			GameUtils::PlayAudioOneShot("confirm.wav");
		}
		else
		{
			memoryMenuWidget->RemoveFromViewport();
			GameUtils::PlayAudioOneShot("cursor.wav");
		}
	}
}

void Player::ExpendActionPoints(int num)
{
	actionPoints -= num;
	actionBarWidget->actionPoints = actionPoints;
}

void Player::LerpPlayerCameraFOV(float deltaTime)
{
	if (camera->FOV != nextCameraFOV)
	{
		camera->FOV = std::lerp(camera->FOV, nextCameraFOV, 4.f * deltaTime);
	}
}

void Player::CheckNextMoveNode(XMVECTOR previousPos)
{
	int nextXIndex = (int)std::round(nextPos.m128_f32[0]);
	int nextYIndex = (int)std::round(nextPos.m128_f32[2]);

	//Keep the call here so player can face walls and holes on input.
	mesh->SetWorldRotation(VMath::LookAtRotation(nextPos, previousPos));

	auto grid = GameUtils::GetGrid();

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
	Ray fenceRay(this);
	if (Raycast(fenceRay, GetPositionVector(), nextPos))
	{
		if (dynamic_cast<FenceActor*>(fenceRay.hitActor))
		{
			nextPos = previousPos;
			return;
		}
	}

	nextPos = XMLoadFloat3(&node->worldPosition);

	if (battleSystem.isBattleActive)
	{
		ExpendActionPoints(1);
	}
}

void Player::SpawnMemoryAsObject()
{
	if (Input::GetKeyUp(Keys::Up))
	{
		if (GameUtils::CheckIfMemoryExists(memoryNameToSpawn))
		{
			Transform transform = GetTransform();
			auto forwardVector = mesh->GetForwardVector();
			transform.position.x += forwardVector.x;
			transform.position.y += forwardVector.y;
			transform.position.z += forwardVector.z;

			auto& memory = GameInstance::playerMemories[memoryNameToSpawn];

			weaponMesh->meshComponentData.filename = memory->meshName;
			weaponMesh->Create();
			weaponMesh->active = true;
		}
		else
		{
			Log("Memory Spawn: [%s] doesn't exist in player's memories.", memoryNameToSpawn.c_str());
		}
	}
}

bool Player::CheckIfPlayerMovementAndRotationStopped()
{
	return XMVector4Equal(GetPositionVector(), nextPos) && XMQuaternionEqual(GetRotationVector(), nextRot);
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
				GameUtils::SetActiveCameraTargetAndZoomOut(this);
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

bool Player::QuickTalkCheck(Actor* hitActor)
{
	if (!isWeaponDrawn&& !inConversation)
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
			return true;
		}
	}
			
	return false;
}

bool Player::InteractCheck(Actor* hitActor)
{
	if (!isWeaponDrawn && !inConversation)
	{
		auto gridActor = dynamic_cast<GridActor*>(hitActor);
		if (gridActor)
		{
			if (gridActor->isInteractable)
			{
				gridActor->Interact();
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
					if (!memory->CreateMemory(gridActor->name))
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
	if (isWeaponDrawn && !inConversation)
	{
		auto unit = dynamic_cast<Unit*>(hitActor);
		if (unit)
		{
			battleSystem.StartBattle();
			ExpendActionPoints(1);
			unit->InflictDamage(1);
			GameUtils::PlayAudioOneShot("sword_hit.wav");
			return true;
		}

		auto gridActor = dynamic_cast<GridActor*>(hitActor);
		if (gridActor)
		{
			gridActor->InflictDamage(1);
			GameUtils::PlayAudioOneShot("sword_hit.wav");
			return true;
		}
	}

	return false;
}

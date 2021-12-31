#include "Player.h"
#include "Components/MeshComponent.h"
#include "Components/EmptyComponent.h"
#include "Components/IntuitionComponent.h"
#include "Camera.h"
#include "Input.h"
#include "VMath.h"
#include "VString.h"
#include "Physics/Raycast.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/NPC.h"
#include "Actors/Game/Pickup.h"
#include "Components/DialogueComponent.h"
#include "Grid.h"
#include "GridActor.h"
#include "UI/HealthWidget.h"
#include "UI/DialogueWidget.h"
#include "UI/InteractWidget.h"
#include "UI/IntuitionMenuWidget.h"
#include "UI/PlayerActionBarWidget.h"
#include "UI/IntuitionTransferWidget.h"
#include "UI/TimeOfDayWidget.h"
#include "UI/HeldPickupWidget.h"
#include "UI/GuardWidget.h"
#include "UI/PlayerHealthWidget.h"
#include "Gameplay/GameInstance.h"
#include "Log.h"
#include "Gameplay/BattleSystem.h"

Player::Player()
{
	nextPos = XMVectorZero();
	nextRot = XMVectorZero();

	//Empty as the root to be able to rotate the mesh towards movement input direction.
	rootComponent = EmptyComponent::system.Add(this);

	mesh = MeshComponent::system.Add(this, MeshComponent("unit_test.fbx", "wall.png"));
	rootComponent->AddChild(mesh);

	camera = CameraComponent::system.Add(this, CameraComponent(XMFLOAT3(1.75f, 2.f, -3.5f), false));

	rootComponent->AddChild(camera);

	dialogueComponent = DialogueComponent::system.Add(this);
}

void Player::Start()
{
	camera->targetActor = this;

	//Setup widgets
	interactWidget = CreateWidget<InteractWidget>();
	intuitionMenuWidget = CreateWidget<IntuitionMenuWidget>();

	actionBarWidget = CreateWidget<PlayerActionBarWidget>();
	actionBarWidget->actionPoints = actionPoints;

	CreateWidget<TimeOfDayWidget>()->AddToViewport();

	heldPickupWidget = CreateWidget<HeldPickupWidget>();
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
		CreateWidget<IntuitionTransferWidget>()->AddToViewport();
		gameOver = true;
	}

	ToggleBattleGrid();
	ToggleIntuitionMenu();

	PrimaryAction();

	PlacePickupDown();

	LerpPlayerCameraFOV(deltaTime);

	dialogueComponent->SetPosition(GetHomogeneousPositionVector());

	//End turn input
	if (battleSystem.isBattleActive && Input::GetKeyUp(Keys::Enter))
	{
		battleSystem.MoveToNextTurn();
	}

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

		ExpendActionPoints(2);

		GameUtils::PlayAudio("equip.wav");
	}
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
		isWeaponDrawn = !isWeaponDrawn;

		//toggle grid
		auto grid = GameUtils::GetGrid();
		if (grid)
		{
			grid->lerpOut = isWeaponDrawn;
			if (grid->lerpOut)
			{
				grid->SetActive(true);
			}
		}

		if (isWeaponDrawn)
		{
			GameUtils::PlayAudio("sword_hit.wav");
			healthWidget->AddToViewport();
		}
		else
		{
			GameUtils::PlayAudio("sword_sheathe.wav");
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
		if (inInteraction)
		{
			interactWidget->RemoveFromViewport();
			inInteraction = false;
			nextCameraFOV = 60.f;
			return;
		}

		Ray ray(this);
		auto forward = mesh->GetForwardVectorV();
		if (Raycast(ray, GetPositionVector(), forward, 1.5f))
		{
			Log("Player interact: %s", ray.hitActor->name.c_str());

			if (DestructibleCheck(ray.hitActor)) { return; }
			if (DialogueCheck(ray.hitActor)) {}
			else if (QuickTalkCheck(ray.hitActor)) {}
			else if (InteractCheck(ray.hitActor)) {}
			else if (PickupCheck(ray.hitActor)) {}
		}
		else
		{
			if (isWeaponDrawn)
			{
				GameUtils::PlayAudio("sword_miss.wav");
			}
		}
	}
}

void Player::ToggleIntuitionMenu()
{
	if (Input::GetKeyUp(Keys::Enter))
	{
		intuitionWidgetToggle = !intuitionWidgetToggle;

		if (intuitionWidgetToggle)
		{
			intuitionMenuWidget->AddToViewport();
			GameUtils::PlayAudio("confirm.wav");
		}
		else
		{
			intuitionMenuWidget->RemoveFromViewport();
			GameUtils::PlayAudio("cursor.wav");
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

	//Rotate player mesh towards next direction
	XMVECTOR lookAtRot = VMath::LookAtRotation(nextPos, previousPos);
	mesh->SetRotation(lookAtRot);

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

	nextPos = XMLoadFloat3(&node->worldPosition);

	ExpendActionPoints(1);
}

void Player::PlacePickupDown()
{
	if (Input::GetKeyUp(Keys::Up))
	{
		Transform transform = GetTransform();
		auto forwardVector = GetForwardVector();
		transform.position.x += forwardVector.x;
		transform.position.y += forwardVector.y;
		transform.position.z += forwardVector.z;

		auto pickup = dynamic_cast<Pickup*>(Pickup::system.SpawnActor(transform));
		pickup->mesh->meshComponentData.filename = GameInstance::pickupSpawnData.meshFilename;
		pickup->CreateAllComponents();

		heldPickupWidget->RemoveFromViewport();
	}
}

bool Player::PickupCheck(Actor* hitActor)
{
	auto pickup = dynamic_cast<Pickup*>(hitActor);
	if (pickup)
	{
		heldItem = pickup;
		GameInstance::pickupSpawnData = PickupSpawnData(pickup);

		//Set pickup widget
		heldPickupWidget->pickupName = heldItem->pickupName;
		heldPickupWidget->AddToViewport();

		pickup->AddToPlayerInventory();
		return true;
	}

	return false;
}

bool Player::DialogueCheck(Actor* hitActor)
{
	if (inConversation)
	{
		if (!dialogueComponent->NextLine())
		{
			//End dialogue
			inConversation = false;
			nextCameraFOV = 60.f;
			GameUtils::SetActiveCameraTarget(this);
			return true;
		}
		else
		{
			if (!dialogueComponent->ShowTextAtActor())
			{
				//Exist out of dialogue loop if next line not found.
				inConversation = false;
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
			dialogueComponent = npc->dialogueComponent;
			if (dialogueComponent->dialogue.filename.empty())
			{
				return false;
			}

			inConversation = true;

			//start dialogue
			dialogueComponent->ShowTextAtActor();

			//Camera zoom and focus
			nextCameraFOV = 30.f;
			GameUtils::SetActiveCameraTarget(npc);

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

bool Player::InteractCheck(Actor* hitActor)
{
	if (!isWeaponDrawn && !inConversation)
	{
		auto gridActor = dynamic_cast<GridActor*>(hitActor);
		if (gridActor)
		{
			if (gridActor->isInteractable)
			{
				interactWidget->interactText = VString::stows(gridActor->interactText);
				interactWidget->AddToViewport();
				inInteraction = true;

				nextCameraFOV = 30.f;

				auto intuition = gridActor->intuition;
				if (intuition->addOnInteract)
				{
					intuition->CreateIntuition(gridActor->name);
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
			GameUtils::PlayAudio("sword_hit.wav");
			return true;
		}

		auto gridActor = dynamic_cast<GridActor*>(hitActor);
		if (gridActor)
		{
			battleSystem.StartBattle();
			ExpendActionPoints(1);
			gridActor->InflictDamage(1);
			GameUtils::PlayAudio("sword_hit.wav");
			return true;
		}
	}

	return false;
}

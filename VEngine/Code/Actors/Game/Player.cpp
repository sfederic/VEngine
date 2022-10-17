#include "vpch.h"
#include "Player.h"
#include "Camera.h"
#include "Input.h"
#include "VMath.h"
#include "Core.h"
#include "Log.h"
#include "Timer.h"
#include "Physics/Raycast.h"
#include "Actors/Game/NPC.h"
#include "Actors/Game/FenceActor.h"
#include "Actors/Game/MemoryCheckGridActor.h"
#include "Grid.h"
#include "GridActor.h"
#include "Components/MeshComponent.h"
#include "Components/EmptyComponent.h"
#include "Components/Game/MemoryComponent.h"
#include "Components/AudioComponent.h"
#include "Components/Game/DialogueComponent.h"
#include "UI/Game/HealthWidget.h"
#include "UI/Game/DialogueWidget.h"
#include "UI/Game/InteractWidget.h"
#include "UI/Game/MemoryMenuWidget.h"
#include "UI/Game/PlayerActionBarWidget.h"
#include "UI/Game/MemoryGainedWidget.h"
#include "UI/Game/MemoryRecalledWidget.h"
#include "UI/Game/BulletWidget.h"
#include "UI/Game/GuardWidget.h"
#include "UI/Game/PlayerHealthWidget.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/BattleSystem.h"
#include "Gameplay/GameUtils.h"
#include "Render/Material.h"

Player::Player()
{
	nextPos = XMVectorZero();
	nextRot = XMVectorZero();

	//Empty as the root to be able to rotate the mesh towards movement input direction.
	rootComponent = EmptyComponent::system.Add("Root", this);

	mesh = MeshComponent::system.Add("Mesh", this, MeshComponent("char.fbx", "test.png"));
	rootComponent->AddChild(mesh);

	camera = CameraComponent::system.Add("Camera", this, CameraComponent(XMFLOAT3(1.75f, 1.75f, -2.75f)));
	camera->targetActor = this;

	rootComponent->AddChild(camera);

	dialogueComponent = DialogueComponent::system.Add("Dialogue", this);

	stepSounds = AudioComponent::system.Add("StepSounds", this);
	stepSounds->loop = true;
	stepSounds->audioFilename = "step.wav";
	stepSounds->playOnStart = true;
}

void Player::Start()
{
	mesh->GetSkeleton()->currentAnimation = "None";

	camera->targetActor = this;

	//Setup widgets
	interactWidget = CreateWidget<InteractWidget>();
	memoryMenuWidget = CreateWidget<MemoryMenuWidget>();

	actionBarWidget = CreateWidget<PlayerActionBarWidget>();
	actionBarWidget->actionPoints = actionPoints;

	guardWidget = CreateWidget<GuardWidget>();

	healthWidget = CreateWidget<PlayerHealthWidget>();

	bulletWidget = CreateWidget<BulletWidget>();
	bulletWidget->AddToViewport();
	bulletWidget->numBulletsPlayerHas = numBullets;

	nextPos = GetPositionV();
	nextRot = GetRotationV();
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
	//@Todo: see if gun range should be a thing
	/*if (gunModeOn && CheckIfPlayerMovementAndRotationStopped())
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
	}*/

	PrimaryAction();
	SecondaryAction();

	ToggleBattleGrid();
	ToggleMemoryMenu();

	LerpPlayerCameraFOV(deltaTime);

	MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	dialogueComponent->SetPosition(GetHomogeneousPositionV());

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
	actionPoints += 5;

	if (actionPoints >= GameInstance::maxPlayerActionPoints)
	{
		actionPoints = GameInstance::maxPlayerActionPoints;
	}

	actionBarWidget->actionPoints = actionPoints;
}

void Player::ResetGuard()
{
	guardSuccess = false;
	guarding = false;
	guardWidget->ResetGuard();
	ableToGuard = true;
}

GridNode* Player::GetCurrentNode()
{
	auto grid = Grid::system.GetFirstActor();
	auto node = grid->GetNode(xIndex, yIndex);
	return node;
}

void Player::InflictDamage(int damage)
{
	int guardPoints = 0;

	if (guarding && guardSuccess)
	{
		Log("Guarded attack");
		guarding = false;
		guardSuccess = false;
		return;
	}

	healthPoints -= damage;

	if (healthPoints <= 0)
	{
		Log("Game Over");
	}
}

void Player::Guard()
{
	if (actionPoints > 0 && !guarding)
	{
		if (Input::GetKeyUp(Keys::Down))
		{
			defendDirection = DefendDirection::Down;
			ConfirmGuardOnDirection();
		}
		else if (Input::GetKeyUp(Keys::Up))
		{
			defendDirection = DefendDirection::Up;
			ConfirmGuardOnDirection();
		}
		else if (Input::GetKeyUp(Keys::Left))
		{
			defendDirection = DefendDirection::Left;
			ConfirmGuardOnDirection();
		}
		else if (Input::GetKeyUp(Keys::Right))
		{
			defendDirection = DefendDirection::Right;
			ConfirmGuardOnDirection();
		}
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

	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, moveSpeed));

	if (CheckIfPlayerMovementAndRotationStopped())
	{
		if (stepSoundsVolume > 0.f) { stepSoundsVolume -= deltaTime * 4.f; }
		stepSounds->volume = stepSoundsVolume;

		xIndex = std::round(GetPosition().x);
		yIndex = std::round(GetPosition().z);

		XMVECTOR previousPos = nextPos;

		if (Input::GetKeyHeld(Keys::W))
		{
			mesh->currentAnimation = "Armature|ArmatureAction";

			nextPos = GetPositionV() + GetForwardVectorV();
			CheckNextMoveNode(previousPos);
		}
		else
		{
			//Testing code for animation states. Keep in mind it would need to be like this for player.
			mesh->currentAnimation.clear();
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

	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, rotSpeed));

	if (CheckIfPlayerMovementAndRotationStopped())
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
		auto grid = Grid::system.GetFirstActor();
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
		if (uiSystem.memoryRecalledWidget->IsInViewport())
		{
			uiSystem.memoryGainedWidget->RemoveFromViewport();
			uiSystem.memoryRecalledWidget->RemoveFromViewport();
			return;
		}

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

		Ray ray(this);
		auto meshForward = mesh->GetForwardVectorV();
		auto center = GetPositionV();
		auto boxCastOrigin = center + meshForward;
		XMFLOAT3 boxCastOriginFloat3;
		XMStoreFloat3(&boxCastOriginFloat3, boxCastOrigin);
		if (SimpleBoxCast(boxCastOriginFloat3, XMFLOAT3(0.25f, 0.25f, 0.25f), ray))
		{
			//@Todo: won't work later on for multiple hit actors
			ray.hitActor = ray.hitActors.front();
			Log("Player interact: %s", ray.hitActor->GetName().c_str());

			if (DestructibleCheck(ray.hitActor)) { return; }
			if (DialogueCheck(ray.hitActor)) {}
			else if (QuickTalkCheck(ray.hitActor)) {}
			else if (InteractCheck(ray.hitActor)) {}
		}
		else
		{
			//@Todo: was causing weird raycast issues. Come back to this for smaller enemies and whatever else.
			//if (!AttackGridActorBasedOnNode())
			{
				if (isWeaponDrawn)
				{
					GameUtils::PlayAudioOneShot("sword_miss.wav");
				}
			}
		}
	}
}

//Using this for secondary action testing where primary doesn't fit.
void Player::SecondaryAction()
{
	if (Input::GetKeyUp(Keys::Up))
	{
		Ray gunRay(this);
		if (Raycast(gunRay, GetPositionV(), mesh->GetForwardVectorV(), 10.f))
		{
			GunShotCheck(gunRay.hitActor);
			return;
		}

		//Ray ray(this);
		//auto meshForward = mesh->GetForwardVectorV();
		//if (Raycast(ray, GetPositionV(), meshForward, 1.5f))
		//{
		//	Log("Player interact: %s", ray.hitActor->GetName().c_str());

		//	if (CombatInteractCheck(ray.hitActor)) {}
		//}
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
			//Toggling off memory widget is handled in its own class because it pauses the game world.
			Core::gameWorldPaused = true;
			memoryMenuWidget->AddToViewport();
			GameUtils::PlayAudioOneShot("confirm.wav");
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
	Ray fenceRay(this);
	if (Raycast(fenceRay, GetPositionV(), nextPos))
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
		PreviewMovementNodesDuringBattle();
		ExpendActionPoints(1);
	}
}

bool Player::CheckIfPlayerMovementAndRotationStopped()
{
	return XMVector4Equal(GetPositionV(), nextPos) && XMQuaternionEqual(GetRotationV(), nextRot);
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

void Player::ConfirmGuardOnDirection()
{
	guarding = true;
	ExpendActionPoints(1);

	//Check if guard matches enemey unit attack direction for successful guard
	guardSuccess = unitCurrentlyAttackingPlayer->attackDirection == defendDirection;

	if (guardSuccess)
	{
		guardWidget->SetGuardSuccess();
		GameUtils::PlayAudioOneShot("equip.wav");
	}
	else
	{
		guardWidget->SetGuardFail();
		GameUtils::PlayAudioOneShot("error.wav");
	}
}

//@Todo: this works fine, there's just a slight polish it needs when you run out of action points and
//preview nodes for the player's movement remains.
void Player::PreviewMovementNodesDuringBattle()
{
	std::vector<GridNode*> movementNodes;
	std::vector<GridNode*> closedNodes;

	GridNode* currentNode = GetCurrentNode();
	Grid* grid = Grid::system.GetFirstActor();

	grid->GetNeighbouringNodes(currentNode, movementNodes);

	for (int moveIndex = 0; moveIndex < (actionPoints - 1); moveIndex++)
	{
		for (int previewIndex = 0; previewIndex < movementNodes.size(); previewIndex++)
		{
			grid->GetNeighbouringNodes(movementNodes[previewIndex], closedNodes);
		}

		movementNodes.insert(movementNodes.end(), closedNodes.begin(), closedNodes.end());

		closedNodes.clear();
	}

	if (actionPoints > 0)
	{
		grid->ResetAllNodes(); //This is more to reset the node colours
	}

	for (auto node : movementNodes)
	{
		node->SetColour(GridNode::previewColour);
	}
}

//Note: Default blend state needs to already be set for the mesh.
void Player::MakeOccludingMeshBetweenCameraAndPlayerTransparent()
{
	static std::vector<Actor*> previousHitActors;

	auto SetActorAlpha = [](Actor* actor, float alpha) {
		auto mesh = actor->GetFirstComponentOfTypeAllowNull<MeshComponent>();
		if (mesh)
		{
			auto ambientColour = mesh->GetAmbientColour();
			ambientColour.w = alpha;
			mesh->SetAmbientColour(ambientColour);
		}
	};

	const float transparentValue = 0.35f;
	const float solidValue = 1.f;

	Ray ray(this);
	if (Raycast(ray, camera->GetWorldPositionV(), GetPositionV()))
	{
		for (auto actor : previousHitActors)
		{
			SetActorAlpha(actor, solidValue);
		}

		for (auto actor : ray.hitActors)
		{
			SetActorAlpha(actor, transparentValue);
		}

		previousHitActors = ray.hitActors;
	}
	else
	{
		for(auto actor : previousHitActors)
		{
			SetActorAlpha(actor, solidValue);
		}

		previousHitActors.clear();
	}
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
			gridActorInteractingWith = gridActor;

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
	if (isWeaponDrawn && !inConversation)
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
			ExpendActionPoints(1);
			GameUtils::CameraShake(1.f);
			GameUtils::SpawnSpriteSheet("Sprites/blood_hit.png", unit->GetPosition(), false, 4, 4);
			GameUtils::PlayAudioOneShot("sword_hit.wav");
			unit->InflictDamage(attackPoints);
			return true;
		}

		auto gridActor = dynamic_cast<GridActor*>(hitActor);
		if (gridActor)
		{
			GameUtils::CameraShake(1.f);
			GameUtils::SpawnSpriteSheet("Sprites/blood_hit.png", gridActor->GetPosition(), false, 4, 4);
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

bool Player::GunShotCheck(Actor* hitActor)
{
	if (isWeaponDrawn && !inConversation)
	{
		auto unit = dynamic_cast<Unit*>(hitActor);
		if (unit)
		{
			battleSystem.StartBattle();
			ExpendActionPoints(1);
			unit->InflictDamage(attackPoints);

			numBullets--;
			bulletWidget->numBulletsPlayerHas = numBullets;
			GameUtils::PlayAudioOneShot("gunshot.wav");

			return true;
		}

		auto gridActor = dynamic_cast<GridActor*>(hitActor);
		if (gridActor)
		{
			gridActor->InflictDamage(attackPoints);
			numBullets--;
			bulletWidget->numBulletsPlayerHas = numBullets;
			GameUtils::PlayAudioOneShot("gunshot.wav");
			return true;
		}
	}

	return false;
}

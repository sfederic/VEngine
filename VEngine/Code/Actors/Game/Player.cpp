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
#include "Actors/Game/GridMapPicker.h"
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
#include "UI/Game/GuardWidget.h"
#include "UI/Game/PlayerHealthWidget.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/BattleSystem.h"
#include "Gameplay/GameUtils.h"
#include "Render/Material.h"

#include "Gameplay/TrapNodes/DamageTrapNode.h"
#include "Actors/Game/AllyUnits/AttackUnit.h"
#include "Gameplay/PlayerInputController.h"

Player::Player()
{
	nextPos = XMVectorZero();
	nextRot = XMVectorZero();

	mesh = CreateComponent(MeshComponent("char.fbx", "test.png"), "Mesh");
	rootComponent->AddChild(mesh);

	dialogueComponent = DialogueComponent::system.Add("Dialogue", this);
}

void Player::Start()
{
	__super::Start();

	activePlayerUnits.push_back(this);

	nextCameraFOV = camera->FOV;

	//Setup widgets
	interactWidget = CreateWidget<InteractWidget>();
	memoryMenuWidget = CreateWidget<MemoryMenuWidget>();

	actionBarWidget = CreateWidget<PlayerActionBarWidget>();
	actionBarWidget->actionPoints = actionPoints;

	guardWidget = CreateWidget<GuardWidget>();

	healthWidget = CreateWidget<PlayerHealthWidget>();
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
		GameUtils::TriggerGameOver();
	}

	PlaceTrap();
	DisarmTrap();

	PrimaryAction();
	SummonAllyUnit();
	SwitchInputBetweenAllyUnitsAndPlayer();

	if (Input::GetKeyUp(Keys::O))
	{
		PushbackObject();
	}

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
	}

	ActivateGridMapPicker();
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
	guarding = false;
	guardWidget->ResetGuard();
	ableToGuard = true;
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

	if (healthPoints <= 0)
	{
		Log("Game Over");
	}
}

void Player::Guard()
{
	if (actionPoints > 0 && !guarding)
	{
		guarding = true;
		ExpendActionPoints(1);

		guardWidget->SetGuardSuccess();
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

void Player::ToggleBattleGrid()
{
	if (Input::GetKeyUp(Keys::Space))
	{
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

	if (Input::GetKeyUp(Keys::Down))
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

		if (uiSystem.memoryRecalledWidget->IsInViewport() 
			|| uiSystem.memoryGainedWidget->IsInViewport())
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

	Ray ray(this);
	if (OrientedBoxCast(ray, camera->GetWorldPositionV(), GetPositionV(), XMFLOAT2(0.5f, 0.5f), true))
	{
		std::vector<Actor*> ableActors;

		for (auto actor : previousHitTransparentActors)
		{
			SetActorAlpha(actor, solidValue);
		}

		for (auto actor : ray.hitActors)
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

void Player::ActivateGridMapPicker()
{
	if (Input::GetKeyUp(Keys::I))
	{
		Transform transform;
		transform.position = GetPosition();
		transform.rotation = GetRotation();
		auto gridMapPicker = GridMapPicker::system.Add(GridMapPicker(), transform);

		gridMapPicker->camera->targetActor = gridMapPicker;
		GameUtils::SetActiveCamera(gridMapPicker->camera);

		Grid::system.GetFirstActor()->SetActive(true);

		SetTickEnabled(false);
	}
}

//Places a trap node on the current node the player is standing on.
void Player::PlaceTrap()
{
	if (Input::GetKeyUp(Keys::T))
	{
		auto currentNode = GetCurrentNode();
		
		if (currentNode->trapNode != nullptr)
		{
			delete currentNode->trapNode;
			currentNode->trapNode = nullptr;
		}

		currentNode->trapNode = new DamageTrapNode();
		currentNode->SetColour(GridNode::trapNodeColour);
	}
}

//Delete trap and set node colour back to normal
void Player::DisarmTrap()
{
	if (Input::GetKeyUp(Keys::Y))
	{
		auto currentNode = GetCurrentNode();

		if (currentNode->trapNode != nullptr)
		{
			delete currentNode->trapNode;
			currentNode->trapNode = nullptr;
		}

		currentNode->SetColour(GridNode::normalColour);
	}
}

//Testing code for a single type
void Player::SummonAllyUnit()
{
	if (Input::GetKeyUp(Keys::Up))
	{
		Transform transform;
		XMStoreFloat3(&transform.position, GetPositionV() + mesh->GetForwardVectorV());
		transform.rotation = mesh->GetRotation();

		auto attackUnit = AttackUnit::system.Add(AttackUnit(), transform);
		attackUnit->Start();

		playerInputController.SetPlayerUnitToControl(attackUnit);

		activePlayerUnits.push_back(attackUnit);
		activePlayerUnitIndex = activePlayerUnits.size() - 1;
		playerInputController.SetPlayerUnitToControl(attackUnit);
	}
}

void Player::SwitchInputBetweenAllyUnitsAndPlayer()
{
	if (Input::GetKeyUp(Keys::X))
	{
		if (activePlayerUnitIndex < activePlayerUnits.size() - 1)
		{
			activePlayerUnitIndex++;

			PlayerUnit* nextUnitToControl = activePlayerUnits[activePlayerUnitIndex];

			//make switching between ally units and player less jarring by using previous unit rotation
			XMVECTOR previousPlayerUnitMeshRotation = nextUnitToControl->mesh->GetRotationV();
			nextUnitToControl->SetRotation(GetRotationV());
			nextUnitToControl->nextRot = GetRotationV();
			nextUnitToControl->mesh->SetRotation(previousPlayerUnitMeshRotation);

			playerInputController.SetPlayerUnitToControl(nextUnitToControl);
		}
		else
		{
			activePlayerUnitIndex = 0;

			playerInputController.SetPlayerUnitToControl(this);
		}
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

			if (CheckAttackPositionAgainstUnitDirection(unit))
			{
				ExpendActionPoints(1);
				GameUtils::CameraShake(1.f);
				GameUtils::SpawnSpriteSheet("Sprites/blood_hit.png", unit->GetPosition(), false, 4, 4);
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

void Player::PushbackObject()
{
	Ray ray(this);
	XMVECTOR end = mesh->GetWorldPositionV() + mesh->GetForwardVectorV();
	if (Raycast(ray, mesh->GetWorldPositionV(), end))
	{
		Actor* hitActor = ray.hitActor;
		auto gridActor = dynamic_cast<GridActor*>(hitActor);
		if (gridActor == nullptr)
		{
			Log("Cannot pushback a non GridActor. Actor [%s].", hitActor->GetName().c_str());
			return;
		}

		if (!gridActor->Pushback(mesh->GetForwardVectorV()))
		{
			Log("Actor [%s] pushedback but did not hit anything.", gridActor->GetName().c_str());
		}
	}
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

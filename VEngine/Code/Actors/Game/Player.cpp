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
#include "UI/Game/PlayerHealthWidget.h"
#include "UI/Game/BattleCardHandWidget.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/BattleSystem.h"
#include "Gameplay/BattleCards/TrapCard.h"
#include "Gameplay/BattleCards/BattleCardSystem.h"
#include "Gameplay/GameUtils.h"
#include "Render/Material.h"
#include "Render/BlendStates.h"
#include "Actors/Game/AllyUnits/AttackUnit.h"

Player::Player()
{
	nextPos = XMVectorZero();
	nextRot = XMVectorZero();

	mesh = CreateComponent("Mesh", MeshComponent("char.vmesh", "test.png"));
	rootComponent->AddChild(mesh);

	dialogueComponent = DialogueComponent::system.Add("Dialogue", this);

	isMainPlayer = true;
}

void Player::Start()
{
	__super::Start();

	nextCameraFOV = camera->FOV;
	camera->SetAsActiveCamera();

	//Setup widgets
	interactWidget = UISystem::CreateWidget<InteractWidget>();
	memoryMenuWidget = UISystem::CreateWidget<MemoryMenuWidget>();

	battleSystem.actionBarWidget = UISystem::CreateWidget<PlayerActionBarWidget>();
	battleSystem.actionBarWidget->actionPoints = battleSystem.playerActionPoints;

	healthWidget = UISystem::CreateWidget<PlayerHealthWidget>();

	battleCardHandWidget = UISystem::CreateWidget<BattleCardHandWidget>();
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


	ControllerInput(deltaTime);

	DrawBattleCard();
	PrimaryAction();
	SwitchInputBetweenAllyUnitsAndPlayer();
	EnterAstralMode();
	ToggleMemoryMenu();

	LerpPlayerCameraFOV(deltaTime);
	MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	dialogueComponent->SetPosition(GetHomogeneousPositionV());

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
}

Properties Player::GetProps()
{
    auto props = Actor::GetProps();
	props.title = "Player";
	return props;
}

void Player::RefreshCombatStats()
{
	battleSystem.playerActionPoints += 5;

	if (battleSystem.playerActionPoints >= GameInstance::maxPlayerActionPoints)
	{
		battleSystem.playerActionPoints = GameInstance::maxPlayerActionPoints;
	}

	battleSystem.actionBarWidget->actionPoints = battleSystem.playerActionPoints;
}

void Player::BattleCleanup()
{
	inAstralMode = false;
	battleSystem.isPlayerTurn = false;

	RefreshCombatStats();

	playerInputController.SetPlayerUnitToControl(this);

	battleCardsInHand.clear();

	auto playerUnits = World::GetAllActorsOfTypeInWorld<PlayerUnit>();
	for (auto playerUnit : playerUnits)
	{
		if (!playerUnit->isMainPlayer)
		{
			auto as = playerUnit->GetActorSystem();
			as->RemoveInterfaceActor(playerUnit);
		}
	}

	healthWidget->RemoveFromViewport();

	GameUtils::SetActiveCamera(camera);
	GameUtils::SetActiveCameraTarget(this);

	DestroyPlayerPhysicalBodyDoubleAndReturnPlayerPosition();
}

void Player::SetupForBattle()
{
	battleSystem.isPlayerTurn = true;
	inAstralMode = true;

	healthWidget->AddToViewport();

	DrawTurnBattleCardHand();
	battleCardHandWidget->AddToViewport();
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
		inAstralMode = !inAstralMode;

		if (inAstralMode)
		{
			SpawnPhysicalRepresentationOfAstralPlayer();

			GameUtils::PlayAudioOneShot("sword_hit.wav");
			healthWidget->AddToViewport();

			battleSystem.StartBattle();
		}
		else
		{
			DestroyPlayerPhysicalBodyDoubleAndReturnPlayerPosition();

			GameUtils::PlayAudioOneShot("sword_sheathe.wav");
			healthWidget->RemoveFromViewport();
		}

		//toggle grid
		auto grid = Grid::system.GetFirstActor();
		if (grid)
		{
			switch (inAstralMode)
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
			if (inAstralMode)
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

	ActivateFirstBattleCardInHand();

	if (Input::GetKeyUp(Keys::Down))
	{
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
		}
		else
		{
			//@Todo: was causing weird raycast issues. Come back to this for smaller enemies and whatever else.
			//if (!AttackGridActorBasedOnNode())
			{
				if (inAstralMode)
				{
					GameUtils::PlayAudioOneShot("sword_miss.wav");
				}
			}
		}
	}
}

void Player::ToggleMemoryMenu()
{
	if (inAstralMode || battleSystem.isBattleActive) return;

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

void Player::SwitchInputBetweenAllyUnitsAndPlayer()
{
	if (Input::GetKeyUp(Keys::X))
	{
		if (activePlayerUnitIndex < activePlayerUnits.size() - 1)
		{
			activePlayerUnitIndex++;

			PlayerUnit* nextUnitToControl = activePlayerUnits[activePlayerUnitIndex];

			//make switching between ally units and player less jarring by using previous unit rotation
			XMVECTOR previousPlayerUnitMeshRotation = nextUnitToControl->mesh->GetWorldRotationV();
			nextUnitToControl->SetRotation(GetRotationV());
			nextUnitToControl->nextRot = GetRotationV();
			nextUnitToControl->mesh->SetWorldRotation(previousPlayerUnitMeshRotation);

			playerInputController.SetPlayerUnitToControl(nextUnitToControl);
		}
		else
		{
			activePlayerUnitIndex = 0;

			playerInputController.SetPlayerUnitToControl(this);
		}
	}
}

void Player::DrawBattleCard()
{
	if (battleSystem.isBattleActive)
	{
		if (Input::GetKeyDown(Keys::Num1))
		{
			auto card = BattleCardSystem::Get().DrawCardAtRandom();
			battleCardsInHand.push_back(card);
		}
	}
}

void Player::ActivateFirstBattleCardInHand()
{
	if (Input::GetKeyUp(Keys::Up))
	{
		if (!battleCardsInHand.empty())
		{
			auto trapCard = dynamic_cast<TrapCard*>(battleCardsInHand.front());
			if (trapCard)
			{
				trapCard->Set();
			}
			else
			{
				battleCardsInHand.front()->Activate();
			}
		}
	}
}

void Player::DrawTurnBattleCardHand()
{
	for (int i = battleCardsInHand.size(); i < battleCardHandSizeLimit; i++)
	{
		auto card = BattleCardSystem::Get().DrawCardAtRandom();
		battleCardsInHand.push_back(card);
	}
}

bool Player::QuickTalkCheck(Actor* hitActor)
{
	if (!inAstralMode && !inConversation)
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
	if (!inAstralMode && !inConversation)
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
	if (inAstralMode && !inConversation)
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
				CheckAndExpendActionPoints(1);
				GameUtils::CameraShake(1.f);
				GameUtils::SpawnSpriteSheet("Sprites/blood_hit.png", unit->GetPositionV(), false, 4, 4);
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
			GameUtils::SpawnSpriteSheet("Sprites/blood_hit.png", gridActor->GetPositionV(), false, 4, 4);
			GameUtils::PlayAudioOneShot("sword_hit.wav");

			gridActor->InflictDamage(attackPoints);

			return true;
		}
	}

	return false;
}

void Player::SpawnPhysicalRepresentationOfAstralPlayer()
{
	mesh->SetShaderFilenames(ShaderItems::Default);
	mesh->SetBlendState(BlendStates::Default);
	mesh->GetMaterial().materialShaderData.ambient = XMFLOAT4(0.1f, 0.1f, 0.9f, 0.5f);

	playerBodyMesh = MeshComponent::system.Add("PlayerBody", nullptr, MeshComponent("char.vmesh", "test.png"));
	playerBodyMesh->Create();
	playerBodyMesh->SetWorldPosition(GetPositionV());
	playerBodyMesh->SetWorldRotation(mesh->GetWorldRotationV());
}

void Player::DestroyPlayerPhysicalBodyDoubleAndReturnPlayerPosition()
{
	SetPosition(playerBodyMesh->GetWorldPositionV());
	nextPos = GetPositionV();

	SetRotation(playerBodyMesh->GetWorldRotationV());
	nextRot = GetRotationV();

	playerBodyMesh->Remove();
	playerBodyMesh = nullptr;

	mesh->SetShaderFilenames(ShaderItems::Default);
	mesh->SetBlendState(BlendStates::null);
	mesh->GetMaterial().materialShaderData.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
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

void Player::PlaceTrap(TrapCard* trapCard)
{
	auto currentNode = GetCurrentNode();
	if (currentNode->trapCard == nullptr)
	{
		currentNode->trapCard = trapCard;
		currentNode->trapCard->connectedNode = currentNode;
		currentNode->SetColour(GridNode::trapNodeColour);
	}
}

void Player::AddCardToHand(BattleCard* card)
{
	battleCardsInHand.emplace_back(card);
}

#include "Player.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/IntuitionComponent.h"
#include "Camera.h"
#include "Input.h"
#include "VMath.h"
#include "VString.h"
#include "Physics/Raycast.h"
#include "Gameplay/GameUtils.h"
#include "Editor/Editor.h"
#include "Actors/Game/NPC.h"
#include "Actors/Game/Pickup.h"
#include "Components/DialogueComponent.h"
#include "Actors/Game/BattleGrid.h"
#include "Actors/Game/GridActor.h"
#include "UI/UISystem.h"
#include "UI/HealthWidget.h"
#include "UI/DialogueWidget.h"
#include "UI/InteractWidget.h"
#include "UI/IntuitionMenuWidget.h"
#include "UI/PlayerActionBarWidget.h"
#include "Gameplay/Intuition.h"
#include "Gameplay/ConditionSystem.h"
#include "Gameplay/GameInstance.h"

DialogueComponent* dialogueComponent;

Player::Player()
{
	nextPos = XMVectorZero();
	nextRot = XMVectorZero();

	mesh = MeshComponent::system.Add(this, MeshComponent("cube.fbx", "wall.png"));
	rootComponent = mesh;

	camera = CameraComponent::system.Add(this, CameraComponent(XMFLOAT3(4.f, 4.f, -9.f), false));

	rootComponent->AddChild(camera);

	widget = WidgetComponent::system.Add(this);
}

void Player::Start()
{
	camera->targetActor = this;


	//Setup widgets
	widget->widget = uiSystem.CreateWidget<DialogueWidget>();
	interactWidget = uiSystem.CreateWidget<InteractWidget>();
	intuitionMenuWidget = uiSystem.CreateWidget<IntuitionMenuWidget>();

	actionBarWidget = uiSystem.CreateWidget<PlayerActionBarWidget>();
	actionBarWidget->actionPoints = actionPoints;


	nextPos = GetPositionVector();
	nextRot = GetRotationVector();
}

void Player::Tick(float deltaTime)
{
	ToggleBattleGrid();
	ToggleIntuitionMenu();

	PrimaryAction();

	//End turn input
	if (Input::GetKeyUp(Keys::Enter))
	{
		auto battleGrid = GameUtils::GetBattleGrid();
		if (battleGrid)
		{
			battleGrid->isPlayerTurn = false;
			isPlayerTurn = false;
		}
	}

	if (inCombat)
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
		//if (inCombat && !isPlayerTurn)
		//{
		//	return;
		//}

		//if (inCombat && actionPoints <= 0)
		//{
		//	return;
		//}

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

void Player::CreateIntuition(IntuitionComponent* intuitionComponent, std::string actorAquiredFromName)
{
	auto intuitionIt = intuitions.find(intuitionComponent->intuitionName);
	if (intuitionIt != intuitions.end())
	{
		editor->Log("%s Intuition already known.", intuitionComponent->intuitionName.c_str());
		return;
	}

	auto intuition = new Intuition();
	intuition->name = intuitionComponent->intuitionName;
	intuition->description = intuitionComponent->intuitionDescription;

	intuition->actorAquiredFrom = actorAquiredFromName;
	intuition->worldAquiredFrom = world.worldFilename;

	intuition->hourAquired = GameInstance::currentHour;
	intuition->minuteAquired = GameInstance::currentMinute;

	//Check if intuition condition passes
	if (!intuitionComponent->condition.empty())
	{
		intuition->conditionFunc = conditionSystem.FindCondition(intuitionComponent->condition);
		intuition->conditionFuncName = intuitionComponent->condition;

		if (intuition->conditionFunc())
		{
			intuitions.emplace(intuition->name, intuition);
			//TODO: change these logs here to Widget->AddToViewport()'s
			editor->Log("%s Intuition created.", intuition->name.c_str());
		}
		else
		{
			delete intuition;
		}
	}
	else
	{
		intuitions.emplace(intuition->name, intuition);
		editor->Log("%s Intuition created.", intuition->name.c_str());
	}
}

void Player::MovementInput(float deltaTime)
{
	const float moveSpeed = 5.5f;
	SetPosition(VMath::VectorConstantLerp(GetPositionVector(), nextPos, deltaTime, moveSpeed));

	if (XMVector4Equal(GetPositionVector(), nextPos) && XMQuaternionEqual(GetRotationVector(), nextRot))
	{
		XMVECTOR previousPos = nextPos;

		if (Input::GetAsyncKey(Keys::W))
		{
			ExpendActionPoints(1);
			nextPos = GetPositionVector() + GetForwardVectorV();
		}
		if (Input::GetAsyncKey(Keys::S))
		{
			ExpendActionPoints(1);
			nextPos = GetPositionVector() + -GetForwardVectorV();
		}
		if (Input::GetAsyncKey(Keys::A))
		{
			ExpendActionPoints(1);
			nextPos = GetPositionVector() + -GetRightVectorV();
		}
		if (Input::GetAsyncKey(Keys::D))
		{
			ExpendActionPoints(1);
			nextPos = GetPositionVector() + GetRightVectorV();
		}

		if (!XMVector4Equal(previousPos, nextPos))
		{
			Ray ray(this);
			XMVECTOR direction = XMVector3Normalize(nextPos - previousPos);
			if (Raycast(ray, GetPositionVector(), direction, 1.f))
			{
				nextPos = previousPos;
			}
		}
	}
}

void Player::RotationInput(float deltaTime)
{
	const float rotSpeed = 5.0f;
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
		inCombat = !inCombat;

		//toggle battlegrid
		auto battleGrid = GameUtils::GetBattleGrid();
		if (battleGrid)
		{
			battleGrid->isPlayerTurn = !battleGrid->isPlayerTurn;
			isPlayerTurn = battleGrid->isPlayerTurn;
			battleGrid->ToggleActive();
		}

		//toggle all health widgets on
		auto healthWidgets = uiSystem.GetAllWidgetsOfType<HealthWidget>();
		for (auto healthWidget : healthWidgets)
		{
			if (inCombat)
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
	if (Input::GetKeyUp(Keys::Down))
	{
		if (inInteraction)
		{
			interactWidget->RemoveFromViewport();
			inInteraction = false;
			return;
		}

		Ray ray(this);
		if (Raycast(ray, GetPositionVector(), GetForwardVectorV(), 1.5f))
		{
			//TODO: all these checks here are just for testing. throw them into functions later.
			
			//PICKUP CHECK
			{
				auto pickup = dynamic_cast<Pickup*>(ray.hitActor);
				if (pickup)
				{
					heldItem = pickup;
					pickup->AddToPlayerInventory();
					return;
				}
			}

			//QUICK DIALOGUE INTERACT CHECK
			{
				if (!inCombat)
				{
					auto npc = dynamic_cast<NPC*>(ray.hitActor);
					if (npc)
					{
						if (npc->isInteractable)
						{
							npc->QuickTalkTo();
							return;
						}
					}
				}
			}

			//INTERACT CHECK
			{
				if (!inCombat)
				{
					auto gridActor = dynamic_cast<GridActor*>(ray.hitActor);
					if (gridActor)
					{
						if (gridActor->isInteractable)
						{
							interactWidget->interactText = stows(gridActor->interactText);
							interactWidget->AddToViewport();
							inInteraction = true;

							auto intuition = gridActor->intuition;
							if (intuition->addOnInteract)
							{
								CreateIntuition(gridActor->intuition, gridActor->name);
							}

							return;
						}
					}
				}
			}

			//DESTRUCTIBLE CHECK
			{
				if (inCombat)
				{
					auto gridActor = dynamic_cast<GridActor*>(ray.hitActor);
					if (gridActor)
					{
						ExpendActionPoints(1);
						gridActor->InflictDamage(1);
						return;
					}
				}
			}

			//DIALOGUE CHECK
			if (inConversation)
			{
				if (!dialogueComponent->NextLine())
				{
					inConversation = false;
				}
				else
				{
					dialogueComponent->ShowTextAtActor();
				}
			}
			else
			{
				NPC* npc = dynamic_cast<NPC*>(ray.hitActor);
				if (npc)
				{
					dialogueComponent = npc->dialogueComponent;
					inConversation = true;
					dialogueComponent->ShowTextAtActor();
				}
			}
		}
	}
}

void Player::ToggleIntuitionMenu()
{
	if (Input::GetKeyUp(Keys::I))
	{
		intuitionWidgetToggle = !intuitionWidgetToggle;

		if (intuitionWidgetToggle)
		{
			intuitionMenuWidget->AddToViewport();
		}
		else
		{
			intuitionMenuWidget->RemoveFromViewport();
		}
	}
}

void Player::ExpendActionPoints(int num)
{
	actionPoints -= num;
	actionBarWidget->actionPoints = actionPoints;
}

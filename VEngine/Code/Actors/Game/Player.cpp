#include "Player.h"
#include "Components/MeshComponent.h"
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
#include "Actors/Game/BattleGrid.h"
#include "Actors/Game/GridActor.h"
#include "UI/HealthWidget.h"
#include "UI/DialogueWidget.h"
#include "UI/InteractWidget.h"
#include "UI/IntuitionMenuWidget.h"
#include "UI/PlayerActionBarWidget.h"
#include "Gameplay/Intuition.h"
#include "Gameplay/ConditionSystem.h"
#include "Gameplay/GameInstance.h"
#include "Log.h"
#include "Gameplay/BattleSystem.h"

DialogueComponent* dialogueComponent;

Player::Player()
{
	nextPos = XMVectorZero();
	nextRot = XMVectorZero();

	mesh = MeshComponent::system.Add(this, MeshComponent("unit_test.fbx", "wall.png"));
	rootComponent = mesh;

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

	nextPos = GetPositionVector();
	nextRot = GetRotationVector();
	nextCameraFOV = camera->FOV;

	xIndex = std::round(GetPosition().x);
	yIndex = std::round(GetPosition().z);
}

void Player::Tick(float deltaTime)
{
	ToggleBattleGrid();
	ToggleIntuitionMenu();

	PrimaryAction();

	LerpPlayerCameraFOV(deltaTime);

	//End turn input
	if (Input::GetKeyUp(Keys::Enter))
	{
		isPlayerTurn = false;
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
		Log("%s Intuition already known.", intuitionComponent->intuitionName.c_str());
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
			Log("%s Intuition created.", intuition->name.c_str());
		}
		else
		{
			delete intuition;
		}
	}
	else
	{
		intuitions.emplace(intuition->name, intuition);
		Log("%s Intuition created.", intuition->name.c_str());
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
			nextCameraFOV = 60.f;
			return;
		}

		Ray ray(this);
		auto forward = GetForwardVector();
		if (Raycast(ray, GetPositionVector(), GetForwardVectorV(), 1.5f))
		{
			Log("Player interact: %s", ray.hitActor->name.c_str());

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

							nextCameraFOV = 30.f;

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
					auto unit = dynamic_cast<Unit*>(ray.hitActor);
					if (unit)
					{
						battleSystem.StartBattle();
					}

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
					nextCameraFOV = 60.f;
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

					//Zoom camera in
					nextCameraFOV = 30.f;
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

void Player::LerpPlayerCameraFOV(float deltaTime)
{
	if (camera->FOV != nextCameraFOV)
	{
		camera->FOV = std::lerp(camera->FOV, nextCameraFOV, 4.f * deltaTime);
	}
}

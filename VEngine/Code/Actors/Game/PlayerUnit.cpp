#include "vpch.h"
#include "PlayerUnit.h"
#include "Core/VMath.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include "Components/MeshComponent.h"
#include "Components/CameraComponent.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/GridMapPicker.h"
#include "Actors/Game/FenceActor.h"
#include "Actors/Game/Unit.h"
#include "Physics/Raycast.h"
#include "Gameplay/BattleSystem.h"
#include "Gameplay/GridNode.h"
#include "Gameplay/GameUtils.h"
#include "UI/Game/PlayerActionBarWidget.h"
#include "UI/Game/PlayerStatusWidget.h"

PlayerUnit::PlayerUnit()
{
	SetEmptyRootComponent();

	camera = CreateComponent("Camera", CameraComponent(XMFLOAT3(1.75f, 1.75f, -2.75f)));
	rootComponent->AddChild(camera);
}

void PlayerUnit::Start()
{
	nextPos = GetPositionV();
	nextRot = GetRotationV();

	SetGridIndices();

	camera->targetActor = this;
}

void PlayerUnit::Tick(float deltaTime)
{
	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, moveSpeed));
	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, rotateSpeed));
}

void PlayerUnit::ControllerInput(float deltaTime)
{
	MovementInput(deltaTime);
	RotationInput(deltaTime);

	Attack();
}

void PlayerUnit::CheckNextMoveNode(XMVECTOR previousPos)
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
		PreviewMovementNodesDuringBattle();
		ExpendActionPoint();
	}
}

//@Todo: this works fine, there's just a slight polish it needs when you run out of action points and
//preview nodes for the playerunits' movement remains.
void PlayerUnit::PreviewMovementNodesDuringBattle()
{
	std::vector<GridNode*> movementNodes;
	std::vector<GridNode*> closedNodes;

	GridNode* currentNode = GetCurrentNode();
	Grid* grid = Grid::system.GetFirstActor();

	grid->GetNeighbouringNodes(currentNode, movementNodes);

	for (int moveIndex = 0; moveIndex < (battleSystem.playerActionPoints - 1); moveIndex++)
	{
		for (int previewIndex = 0; previewIndex < movementNodes.size(); previewIndex++)
		{
			grid->GetNeighbouringNodes(movementNodes[previewIndex], closedNodes);
		}

		movementNodes.insert(movementNodes.end(), closedNodes.begin(), closedNodes.end());

		closedNodes.clear();
	}

	if (battleSystem.playerActionPoints > 0)
	{
		grid->ResetAllNodes(); //This is more to reset the node colours
	}

	for (auto node : movementNodes)
	{
		if (node->trap == nullptr)
		{
			node->SetColour(GridNode::previewColour);
		}
	}
}

bool PlayerUnit::CheckIfMovementAndRotationStopped()
{
	return XMVector4Equal(GetPositionV(), nextPos) && XMQuaternionEqual(GetRotationV(), nextRot);
}

void PlayerUnit::MovementInput(float deltaTime)
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

void PlayerUnit::RotationInput(float deltaTime)
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

void PlayerUnit::Attack()
{
	if (Input::GetKeyUp(Keys::Down))
	{
		AttackPattern();
	}
}

void PlayerUnit::GetGridIndices(int& x, int& y)
{
	auto pos = GetPosition();
	x = std::lroundf(pos.x);
	y = std::lroundf(pos.z);
}

void PlayerUnit::ToggleGridMapPicker(bool& gridPickerActive)
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

void PlayerUnit::ExpendActionPoint()
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

void PlayerUnit::InflictDamage(int damage)
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

GridNode* PlayerUnit::GetCurrentNode()
{
	auto grid = Grid::system.GetFirstActor();
	auto node = grid->GetNode(xIndex, yIndex);
	return node;
}

void PlayerUnit::SetGridIndices()
{
	xIndex = std::lroundf(GetPosition().x);
	yIndex = std::lroundf(GetPosition().z);
}

#include "vpch.h"
#include "PlayerUnit.h"
#include "VMath.h"
#include "Input.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/FenceActor.h"
#include "Physics/Raycast.h"
#include "Gameplay/BattleSystem.h"
#include "Gameplay/GridNode.h"
#include "UI/Game/PlayerActionBarWidget.h"

void PlayerUnit::Tick(float deltaTime)
{
	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, moveSpeed));
	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, rotSpeed));
}

void PlayerUnit::ControllerInput(float deltaTime)
{
	MovementInput(deltaTime);
	RotationInput(deltaTime);
}

void PlayerUnit::CheckNextMoveNode(XMVECTOR previousPos)
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

//@Todo: this works fine, there's just a slight polish it needs when you run out of action points and
//preview nodes for the playerunits' movement remains.
void PlayerUnit::PreviewMovementNodesDuringBattle()
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
		if (node->trapNode == nullptr)
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
		xIndex = std::round(GetPosition().x);
		yIndex = std::round(GetPosition().z);

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

void PlayerUnit::ExpendActionPoints(int num)
{
	actionPoints -= num;
	actionBarWidget->actionPoints = actionPoints;
}

GridNode* PlayerUnit::GetCurrentNode()
{
	auto grid = Grid::system.GetFirstActor();
	auto node = grid->GetNode(xIndex, yIndex);
	return node;
}

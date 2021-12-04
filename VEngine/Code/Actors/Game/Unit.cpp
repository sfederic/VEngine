#define NOMINMAX

#include "Unit.h"
#include <algorithm>
#include "Gameplay/GridNode.h"
#include "Gameplay/GameUtils.h"
#include "BattleGrid.h"
#include "Components/MeshComponent.h"
#include "VMath.h"
#include "Actors/Game/Player.h"
#include "Gameplay/BattleSystem.h"

Unit::Unit()
{
}

void Unit::Start()
{
	__super::Start();

	//Careful here with the naming (x/z)
	xIndex = std::round(GetPosition().x);
	yIndex = std::round(GetPosition().z);

	nextMovePos = GetPositionVector();
}

void Unit::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (isUnitTurn)
	{
		if (XMVector4Equal(nextMovePos, GetPositionVector()))
		{
			xIndex = std::round(GetPosition().x);
			yIndex = std::round(GetPosition().z);

			if (movementPathNodeIndex < pathNodes.size())
			{
				nextMovePos = XMLoadFloat3(&pathNodes[movementPathNodeIndex]->worldPosition);

				xIndex = pathNodes[movementPathNodeIndex]->xIndex;
				yIndex = pathNodes[movementPathNodeIndex]->yIndex;

				movementPathNodeIndex++;
			}
			else if (movementPathNodeIndex >= pathNodes.size())
			{
				movementPathNodeIndex = 0;
				pathNodes.clear();
				isUnitTurn = false;

				battleSystem.MoveToNextTurn();
			}
		}
	}

	SetPosition(VMath::VectorConstantLerp(GetPositionVector(), nextMovePos, deltaTime, moveSpeed));
}

Properties Unit::GetProps()
{
	auto props = __super::GetProps();
	props.Add("Move Points", &movementPoints);
	props.Add("NextMove", &nextMovePos);
	props.Add("Move Speed", &moveSpeed);
	return props;
}

void Unit::MoveToNode(GridNode* destinationNode)
{
	BattleGrid* battleGrid = GameUtils::GetBattleGrid();
	GridNode* startingNode = battleGrid->GetNode(xIndex, yIndex);

	std::vector<GridNode*> nodes;
	std::vector<GridNode*> closedNodes;

	battleGrid->GetNeighbouringNodes(startingNode, nodes);

	for (int moveIndex = 0; moveIndex < movementPoints; moveIndex++)
	{
		for (int previewIndex = 0; previewIndex < nodes.size(); previewIndex++)
		{
			battleGrid->GetNeighbouringNodes(nodes[previewIndex], closedNodes);
		}

		nodes.insert(nodes.end(), closedNodes.begin(), closedNodes.end());

		closedNodes.clear();
	}

	for (GridNode* node : nodes)
	{
		//reset the closes state
		node->closed = false;

		movementPathNodes.push_back(node);
	}

	//Assign all costs
	for (auto node : movementPathNodes)
	{
		//TODO: come back here and see if getting the distances with just the int x/yIndex values is enough.
		//I'm guessing the Vector distance stuff here was for node height originally.
		XMVECTOR startPos = XMLoadFloat3(&startingNode->worldPosition);
		XMVECTOR endPos = XMLoadFloat3(&destinationNode->worldPosition);
		XMVECTOR currentPos = XMLoadFloat3(&node->worldPosition);

		node->gCost = XMVector3Length(startPos - currentPos).m128_f32[0];
		node->hCost = XMVector3Length(endPos - currentPos).m128_f32[0];
	}


	GridNode* nextNode = nullptr;

	int highestHCostIndex = 0;
	int lowestHCostIndex = 0;

	float lowestHCost = std::numeric_limits<float>::max();
	for (int i = 0; i < movementPathNodes.size(); i++)
	{
		if (movementPathNodes[i]->hCost < lowestHCost)
		{
			lowestHCost = movementPathNodes[i]->hCost;
			lowestHCostIndex = i;
			nextNode = movementPathNodes[i];
		}
	}

	//This needs to be here if you need Units moving exactly to a point, but most times
	// the unit is going to be moving to a surrounding node if there is a target, eg. the unit
	// isn't going to move onto the node of the player, instead to a neighbouring node
	//if (nextNode != nullptr)
	//{
	//	pathNodes.push_back(nextNode);
	//}

	if (nextNode)
	{
		while (nextNode != startingNode)
		{
			if (nextNode->parentNode)
			{
				nextNode = nextNode->parentNode;
				pathNodes.push_back(nextNode);
			}
		}
	}

	std::reverse(pathNodes.begin(), pathNodes.end());

	movementPathNodes.clear();
}

void Unit::MoveToNode(int x, int y)
{
	BattleGrid* battleGrid = GameUtils::GetBattleGrid();
	GridNode* destinationNode = battleGrid->GetNode(x, y);

	MoveToNode(destinationNode);
}

void Unit::StartTurn()
{
	isUnitTurn = true;

	auto player = GameUtils::GetPlayer();
	MoveToNode(player->xIndex, player->yIndex);
}

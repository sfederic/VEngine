#include "Unit.h"
#include <algorithm>
#include "GridNode.h"
#include "GameUtils.h"
#include "BattleGrid.h"
#include "VMath.h"
#include "Components/MeshComponent.h"

#undef max

Unit::Unit()
{
	mesh = MeshComponent::system.Add(this, MeshComponent("cube.fbx", "test.png"));
	rootComponent = mesh;
}

void Unit::Start()
{
	//Careful here with the naming (x/z)
	xIndex = std::round(GetPosition().x);
	yIndex = std::round(GetPosition().z);

	nextMovePos = GetPositionVector();

	BattleGrid* battleGrid = GameUtils::GetBattleGrid();
	GridNode* destNode = battleGrid->GetNode(4, 0);
	MoveTo(destNode);
}

void Unit::Tick(float deltaTime)
{
	if (XMVector4Equal(nextMovePos, GetPositionVector()))
	{
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
		}
	}

	const float moveSpeed = 1.5f;
	SetPosition(VMath::VectorConstantLerp(GetPositionVector(), nextMovePos, deltaTime, moveSpeed));
}

Properties Unit::GetProps()
{
	auto props = Actor::GetProps();
	props.Add("Move Points", &movementPoints);
	props.Add("NextMove", &nextMovePos);
	return props;
}

void Unit::MoveTo(GridNode* destinationNode)
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

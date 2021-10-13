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

void Unit::Tick(double deltaTime)
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
	props.Add("Next Move", &nextMovePos);
	return props;
}

void Unit::MoveTo(GridNode* destinationNode)
{
	BattleGrid* battleGrid = GameUtils::GetBattleGrid();
	GridNode* startingNode = battleGrid->GetNode(xIndex, yIndex);

	std::vector<GridNode*> previewNodes;
	std::vector<GridNode*> closedPreviewNodes;

	battleGrid->GetNeighbouringNodes(startingNode, previewNodes);

	for (int moveIndex = 0; moveIndex < movementPoints; moveIndex++)
	{
		for (int previewIndex = 0; previewIndex < previewNodes.size(); previewIndex++)
		{
			battleGrid->GetNeighbouringNodes(previewNodes[previewIndex], closedPreviewNodes);
		}

		previewNodes.insert(previewNodes.end(), closedPreviewNodes.begin(), closedPreviewNodes.end());
		closedPreviewNodes.clear();
	}

	for (GridNode* node : previewNodes)
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

	if (nextNode != nullptr)
	{
		pathNodes.push_back(nextNode);
	}

	while (nextNode != startingNode)
	{
		if (nextNode->parentNode)
		{
			nextNode = nextNode->parentNode;
			pathNodes.push_back(nextNode);
		}
	}

	std::reverse(pathNodes.begin(), pathNodes.end());

	movementPathNodes.clear();
}

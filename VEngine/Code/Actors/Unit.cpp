#include "Unit.h"
#include <algorithm>
#include "GridNode.h"
#include "GameUtils.h"
#include "BattleGrid.h"

Unit::Unit()
{
}

Properties Unit::GetProps()
{
	return Properties();
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

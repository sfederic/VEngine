#include "vpch.h"
#include "UnitSkill.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/SkillNode.h"

std::vector<SkillNode*> UnitSkill::SpawnSkillNodes(std::vector<GridNode*>& gridNodes)
{
	std::vector<SkillNode*> skillNodes;

	for (GridNode* node : gridNodes)
	{
		Transform transform{};
		transform.position = node->worldPosition;
		transform.position.y -= 0.35f;
		skillNodes.push_back(SkillNode::system.Add(SkillNode(), transform));
	}

	return skillNodes;
}

void UnitSkill::GetOffsetsForTargetQuadrant(GridNode* startingNode, GridNode* targetNode, int& xOffset, int& yOffset)
{
	const int startingX = startingNode->xIndex;
	const int startingY = startingNode->yIndex;

	const int targetX = targetNode->xIndex;
	const int targetY = targetNode->yIndex;

	if (targetX < startingX) xOffset = -1;
	else if (targetX == startingX) xOffset = 0;
	else xOffset = 1;

	if (targetY < startingY) yOffset = -1;
	else if (targetY == startingY) yOffset = 0;
	else yOffset = 1;
}

bool UnitSkill::IsTargetNodeInRange(GridNode* startingNode, GridNode* targetNode)
{
	auto grid = Grid::system.GetFirstActor();
	grid->ResetAllNodes();

	//Get nodes based on skill range
	std::vector<GridNode*> rangeNodes;
	std::vector<GridNode*> closedNodes;
	grid->GetNeighbouringNodes(startingNode, rangeNodes);

	for (int rangeIndex = 0; rangeIndex < range; rangeIndex++)
	{
		for (int nodeIndex = 0; nodeIndex < rangeNodes.size(); nodeIndex++)
		{
			grid->GetNeighbouringNodes(rangeNodes[nodeIndex], closedNodes);
		}

		rangeNodes.insert(rangeNodes.end(), closedNodes.begin(), closedNodes.end());

		closedNodes.clear();
	}

	for (auto node : rangeNodes)
	{
		if (node->Equals(targetNode))
		{
			//skill in range
			return true;
		}
	}

	//skill out of range
	return false;
}

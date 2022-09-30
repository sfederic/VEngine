#include "vpch.h"
#include "UnitSkill.h"
#include "Actors/Game/Grid.h"

bool UnitSkill::GetAllNodesInRange(GridNode* startingNode, GridNode* targetNode)
{
	auto grid = Grid::system.GetFirstActor();

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

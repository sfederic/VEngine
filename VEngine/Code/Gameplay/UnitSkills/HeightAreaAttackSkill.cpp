#include "vpch.h"
#include "HeightAreaAttackSkill.h"
#include "Actors/Game/Grid.h"

HeightAreaAttackSkill::HeightAreaAttackSkill()
{
}

std::vector<SkillNode*> HeightAreaAttackSkill::SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode)
{
	auto grid = Grid::system.GetFirstActor();
	std::vector<GridNode*> nodesWithSameHeightAsUnit;

	for (auto node : grid->GetAllNodes())
	{
		if (node->worldPosition.y == startingNode->worldPosition.y)
		{
			nodesWithSameHeightAsUnit.push_back(node);
		}
	}

	return SpawnSkillNodes(nodesWithSameHeightAsUnit);
}

#include "vpch.h"
#include "HeightAreaAttackSkill.h"
#include "Actors/Game/Grid.h"
#include "VMath.h"

HeightAreaAttackSkill::HeightAreaAttackSkill()
{
}

std::vector<SkillNode*> HeightAreaAttackSkill::SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode)
{
	auto grid = Grid::system.GetFirstActor();
	std::vector<GridNode*> nodesWithSameHeightAsUnit;

	for (auto node : grid->GetAllNodes())
	{
		if (VMath::FloatEqualRange(node->worldPosition.y, startingNode->worldPosition.y, 0.25f))
		{
			nodesWithSameHeightAsUnit.push_back(node);
		}
	}

	return SpawnSkillNodes(nodesWithSameHeightAsUnit);
}

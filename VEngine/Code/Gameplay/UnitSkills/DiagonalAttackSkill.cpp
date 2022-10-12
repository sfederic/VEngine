#include "vpch.h"
#include "DiagonalAttackSkill.h"
#include "Actors/Game/Grid.h"

DiagonalAttackSkill::DiagonalAttackSkill()
{
	range = 3;
}

std::vector<SkillNode*> DiagonalAttackSkill::SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode)
{
	auto grid = Grid::system.GetFirstActor();

	int startingX = startingNode->xIndex;
	int startingY = startingNode->yIndex;

	std::vector<GridNode*> nodes;

	for (int i = 0; i < range; i++)
	{
		startingX -= 1;
		startingY -= 1;

		auto node = grid->GetNodeAllowNull(startingX, startingY);
		if (node != nullptr)
		{
			nodes.push_back(node);
		}
	}

	return SpawnSkillNodes(nodes);
}

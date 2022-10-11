#include "vpch.h"
#include "LineAttackSkill.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/SkillNode.h"

LineAttackSkill::LineAttackSkill()
{
	range = 3;
}

std::vector<SkillNode*> LineAttackSkill::SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode)
{
	Grid* grid = Grid::system.GetFirstActor();

	std::vector<GridNode*> nodes;

	int nextXIndex = startingNode->xIndex - 1; //testing increment

	for (int i = 0; i < range; i++)
	{
		GridNode* node = grid->GetNodeAllowNull(nextXIndex, startingNode->yIndex);
		if (node)
		{
			nodes.push_back(node);
		}
		nextXIndex -= 1;
	}

	return SpawnSkillNodes(nodes);
}

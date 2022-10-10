#include "vpch.h"
#include "AreaAttackSkill.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/SkillNode.h"

std::vector<SkillNode*> AreaAttackSkill::SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode)
{
	Grid* grid = Grid::system.GetFirstActor();

	std::vector<GridNode*> nodes;
	grid->GetNeighbouringNodesForceful(startingNode, nodes);

	return SpawnSkillNodes(nodes);
}

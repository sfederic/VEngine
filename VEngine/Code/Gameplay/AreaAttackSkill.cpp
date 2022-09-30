#include "vpch.h"
#include "AreaAttackSkill.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/SkillNode.h"

void AreaAttackSkill::SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode)
{
	Grid* grid = Grid::system.GetFirstActor();

	std::vector<GridNode*> nodes;
	grid->GetNeighbouringNodesForceful(startingNode, nodes);

	for (GridNode* node : nodes)
	{
		Transform transform{};
		transform.position = node->worldPosition;
		transform.position.y -= 0.35f;
		SkillNode::system.Add(SkillNode(), transform);
	}
}

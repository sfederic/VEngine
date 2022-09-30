#include "vpch.h"
#include "AreaAttackSkill.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/SkillNode.h"

void AreaAttackSkill::SetNodesForSkillRange(int unitIndexX, int unitIndexY)
{
	Grid* grid = Grid::system.GetFirstActor();
	GridNode* unitNode = grid->GetNode(unitIndexX, unitIndexY);

	std::vector<GridNode*> nodes;
	grid->GetNeighbouringNodesForceful(unitNode, nodes);

	for (GridNode* node : nodes)
	{
		Transform transform{};
		transform.position = node->worldPosition;
		transform.position.y -= 0.35f;
		SkillNode::system.Add(SkillNode(), transform);
	}
}

#include "vpch.h"
#include "LineAttackSkill.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/SkillNode.h"

LineAttackSkill::LineAttackSkill()
{
	range = 3;
}

void LineAttackSkill::SetNodesForSkillRange(int unitIndexX, int unitIndexY)
{
	Grid* grid = Grid::system.GetFirstActor();
	GridNode* unitNode = grid->GetNode(unitIndexX, unitIndexY);

	std::vector<GridNode*> nodes;

	int nextXIndex = unitIndexX - 1; //testing increment

	for (int i = 0; i < range; i++)
	{
		GridNode* node = grid->GetNodeAllowNull(nextXIndex, unitIndexY);
		if (node)
		{
			nodes.push_back(node);
		}
		nextXIndex -= 1;
	}

	for (GridNode* node : nodes)
	{
		Transform transform{};
		transform.position = node->worldPosition;
		transform.position.y -= 0.35f;
		SkillNode::system.Add(SkillNode(), transform);
	}
}

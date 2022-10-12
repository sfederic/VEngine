#include "vpch.h"
#include "CrossAttackSkill.h"
#include "Actors/Game/Grid.h"

CrossAttackSkill::CrossAttackSkill()
{
    range = 3;
}

std::vector<SkillNode*> CrossAttackSkill::SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode)
{
    auto grid = Grid::system.GetFirstActor();

	const int startingX = startingNode->xIndex;
	const int startingY = startingNode->yIndex;

	std::vector<GridNode*> nodes;

	GetDiagonalNodes(startingX, startingY, 1, 1, nodes);
	GetDiagonalNodes(startingX, startingY, 1, -1, nodes);
	GetDiagonalNodes(startingX, startingY, -1, 1, nodes);
	GetDiagonalNodes(startingX, startingY, -1, -1, nodes);
	
	return SpawnSkillNodes(nodes);
}

void CrossAttackSkill::GetDiagonalNodes(
	const int x, const int y, const int offsetX, const int offsetY, std::vector<GridNode*>& nodes)
{
	int skillXIndex = x;
	int skillYIndex = y;

	auto grid = Grid::system.GetFirstActor();

	for (int i = 0; i < range; i++)
	{
		skillXIndex += offsetX;
		skillYIndex += offsetY;

		auto node = grid->GetNodeAllowNull(skillXIndex, skillYIndex);
		if (node != nullptr)
		{
			nodes.push_back(node);
		}
	}
}

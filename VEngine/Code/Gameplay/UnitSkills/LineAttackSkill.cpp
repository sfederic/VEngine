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
	auto grid = Grid::system.GetFirstActor();
	std::vector<GridNode*> nodes;

	int xOffset = 0; 
	int yOffset = 0;
	GetOffsetsForTargetQuadrant(startingNode, targetNode, xOffset, yOffset);

	int xIndex = startingNode->xIndex;
	int yIndex = startingNode->yIndex;

	for (int i = 0; i < range; i++)
	{
		xIndex += xOffset;
		yIndex += yOffset;

		GridNode* node = grid->GetNodeAllowNull(xIndex, yIndex);
		if (node != nullptr)
		{
			nodes.push_back(node);
		}
	}

	return SpawnSkillNodes(nodes);
}

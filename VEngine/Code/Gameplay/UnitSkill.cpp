#include "vpch.h"
#include "UnitSkill.h"
#include "Actors/Game/Grid.h"
#include "Actors/Effects/AOELine.h"

void UnitSkill::SetNodesForSkillRange(int unitIndexX, int unitIndexY)
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
		AOELine::system.Add(AOELine(), transform);
	}
}

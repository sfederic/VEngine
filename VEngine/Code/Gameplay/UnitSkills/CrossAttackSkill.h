#pragma once

#include "UnitSkill.h"

//Attacks in a diagonal cross formation starting from the unit.
class CrossAttackSkill : public UnitSkill
{
public:
	CrossAttackSkill();
	virtual std::vector<SkillNode*> SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode) override;

private: 
	void GetDiagonalNodes(
		const int x, const int y, const int offsetX, const int offsetY, std::vector<GridNode*>& nodes);
};

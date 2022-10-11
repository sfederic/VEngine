#pragma once

#include "UnitSkill.h"

//Attacks at every node in the grid.
class EntireAreaAttackSkill : public UnitSkill
{
public:
	EntireAreaAttackSkill();
	virtual std::vector<SkillNode*> SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode) override;
};

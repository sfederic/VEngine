#pragma once

#include "UnitSkill.h"

//Attacks all surrounding nodes that share the same height as the current node of the Unit.
class HeightAreaAttackSkill : public UnitSkill
{
public:
	HeightAreaAttackSkill();
	virtual std::vector<SkillNode*> SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode) override;
};

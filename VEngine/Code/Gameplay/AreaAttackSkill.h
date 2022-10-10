#pragma once

#include "UnitSkill.h"

class AreaAttackSkill : public UnitSkill
{
public:
	virtual std::vector<SkillNode*> SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode) override;
};

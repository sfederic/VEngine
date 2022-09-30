#pragma once

#include "UnitSkill.h"

class AreaAttackSkill : public UnitSkill
{
public:
	virtual void SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode) override;
};

#pragma once

#include "UnitSkill.h"

class LineAttackSkill : public UnitSkill
{
public:
	LineAttackSkill();
	virtual void SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode) override;
};

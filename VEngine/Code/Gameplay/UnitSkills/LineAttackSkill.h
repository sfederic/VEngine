#pragma once

#include "UnitSkill.h"

class LineAttackSkill : public UnitSkill
{
public:
	LineAttackSkill();
	virtual std::vector<SkillNode*> SetNodesForSkillRange(GridNode* startingNode, GridNode* targetNode) override;
};

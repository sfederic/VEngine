#pragma once

#include "UnitSkill.h"

class LineAttackSkill : public UnitSkill
{
public:
	LineAttackSkill();
	virtual void SetNodesForSkillRange(int unitIndexX, int unitIndexY) override;
};

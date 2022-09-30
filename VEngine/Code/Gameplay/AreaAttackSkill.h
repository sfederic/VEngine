#pragma once

#include "UnitSkill.h"

class AreaAttackSkill : public UnitSkill
{
public:
	virtual void SetNodesForSkillRange(int unitIndexX, int unitIndexY) override;
};

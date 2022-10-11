#include "vpch.h"
#include "LineAttackerUnit.h"
#include "Gameplay/UnitSkills/AreaAttackSkill.h"

LineAttackUnit::LineAttackUnit()
{
	skills.emplace("area", std::make_unique<AreaAttackSkill>());
}

#include "vpch.h"
#include "LineAttackerUnit.h"
#include "Gameplay/UnitSkills/LineAttackSkill.h"

LineAttackUnit::LineAttackUnit()
{
	skills.emplace("area", std::make_unique<LineAttackSkill>());
}

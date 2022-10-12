#include "vpch.h"
#include "LineAttackerUnit.h"
#include "Gameplay/UnitSkills/CrossAttackSkill.h"

LineAttackUnit::LineAttackUnit()
{
	skills.emplace("area", std::make_unique<CrossAttackSkill>());
}

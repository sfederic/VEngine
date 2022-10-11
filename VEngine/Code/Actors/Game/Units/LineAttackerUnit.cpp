#include "vpch.h"
#include "LineAttackerUnit.h"
#include "Gameplay/UnitSkills/EntireAreaAttackSkill.h"

LineAttackUnit::LineAttackUnit()
{
	skills.emplace("area", std::make_unique<EntireAreaAttackSkill>());
}

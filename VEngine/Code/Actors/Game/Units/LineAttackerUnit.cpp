#include "vpch.h"
#include "LineAttackerUnit.h"
#include "Gameplay/UnitSkills/HeightAreaAttackSkill.h"

LineAttackUnit::LineAttackUnit()
{
	skills.emplace("area", std::make_unique<HeightAreaAttackSkill>());
}

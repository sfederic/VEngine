#include "vpch.h"
#include "LineAttackerUnit.h"
#include "Gameplay/UnitSkills/DiagonalAttackSkill.h"

LineAttackUnit::LineAttackUnit()
{
	skills.emplace("area", std::make_unique<DiagonalAttackSkill>());
}

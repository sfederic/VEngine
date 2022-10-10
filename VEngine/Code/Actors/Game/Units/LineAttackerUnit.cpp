#include "vpch.h"
#include "LineAttackerUnit.h"
#include "Gameplay/AreaAttackSkill.h"

LineAttackUnit::LineAttackUnit()
{
	skills.emplace("area", std::make_unique<AreaAttackSkill>());
}

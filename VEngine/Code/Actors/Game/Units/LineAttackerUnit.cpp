#include "vpch.h"
#include "LineAttackerUnit.h"
#include "Gameplay/LineAttackSkill.h"

LineAttackUnit::LineAttackUnit()
{
	skills.emplace("line", std::make_unique<LineAttackSkill>());
}

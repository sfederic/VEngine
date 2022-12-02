#include "vpch.h"
#include "ShieldUnit.h"

ShieldUnit::ShieldUnit()
{
	attackDirections = AttackDirection::Back | AttackDirection::Left | AttackDirection::Right;
}

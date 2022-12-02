#include "vpch.h"
#include "DamageTrapNode.h"
#include "Actors/Game/Unit.h"

void DamageTrapNode::Activate(Unit* unit)
{
	unit->InflictDamage(1);
}

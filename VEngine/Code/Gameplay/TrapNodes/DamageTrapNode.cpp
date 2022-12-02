#include "vpch.h"
#include "DamageTrapNode.h"
#include "Actors/Game/Unit.h"

DamageTrapNode::DamageTrapNode()
{
	effectDescription = L"Damages enemy for 1.";
}

void DamageTrapNode::Activate(Unit* unit)
{
	unit->InflictDamage(1);
}

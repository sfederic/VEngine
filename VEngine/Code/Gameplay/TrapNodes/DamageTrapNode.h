#pragma once

#include "TrapNode.h"

class DamageTrapNode : public TrapNode
{
public:
	DamageTrapNode();
	virtual void Activate(Unit* unit) override;
};
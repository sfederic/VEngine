#pragma once

#include "TrapNode.h"

class DamageTrapNode : public TrapNode
{
public:
	virtual void Activate(Unit* unit) override;
};
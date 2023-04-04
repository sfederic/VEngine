#pragma once

#include "../Widget.h"

class Unit;
struct Trap;

class ActivateTrapWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;
	void SetLinkedUnit(Unit* unit) { linkedUnit = unit; }
	void SetLinkedTrapNode(Trap* trap) { linkedTrap = trap; }

private:
	Unit* linkedUnit = nullptr;
	Trap* linkedTrap = nullptr;
};

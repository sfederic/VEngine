#pragma once

#include "../Widget.h"

class Unit;
class TrapNode;

class ActivateTrapWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;
	void SetLinkedUnit(Unit* unit) { linkedUnit = unit; }
	void SetLinkedTrapNode(TrapNode* trapNode) { linkedTrapNode = trapNode; }

private:
	Unit* linkedUnit = nullptr;
	TrapNode* linkedTrapNode = nullptr;
};

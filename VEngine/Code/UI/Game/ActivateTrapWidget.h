#pragma once

#include "../Widget.h"

class Unit;
struct TrapCard;

class ActivateTrapWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;
	void SetLinkedUnit(Unit* unit) { linkedUnit = unit; }
	void SetLinkedTrapNode(TrapCard* trapCard) { linkedTrapCard = trapCard; }

private:
	Unit* linkedUnit = nullptr;
	TrapCard* linkedTrapCard = nullptr;
};

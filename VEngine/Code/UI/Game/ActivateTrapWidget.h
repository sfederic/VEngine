#pragma once

#include "../Widget.h"

class Unit;
class BattleCard;

class ActivateTrapWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;
	void SetLinkedUnit(Unit* unit) { linkedUnit = unit; }
	void SetLinkedTrapNode(BattleCard* trapCard) { linkedTrapCard = trapCard; }

private:
	Unit* linkedUnit = nullptr;
	BattleCard* linkedTrapCard = nullptr;
};

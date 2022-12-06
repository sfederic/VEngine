#pragma once

#include "../Widget.h"

class GridActor;
class BattleCard;

class GridMapPickerSelectionInfoWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

private:
	void DrawGridActorInfo();
	void DrawTrapNodeInfo();

public:
	GridActor* selectedGridActor = nullptr;
	
	BattleCard* selectedTrapCard = nullptr;
};

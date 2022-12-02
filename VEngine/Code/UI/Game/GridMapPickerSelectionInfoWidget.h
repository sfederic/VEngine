#pragma once

#include "../Widget.h"

class GridActor;
class TrapNode;

class GridMapPickerSelectionInfoWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

private:
	void DrawGridActorInfo();
	void DrawTrapNodeInfo();

public:
	GridActor* selectedGridActor = nullptr;
	
	TrapNode* selectedTrapNode = nullptr;
};

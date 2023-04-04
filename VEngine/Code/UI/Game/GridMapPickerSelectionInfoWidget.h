#pragma once

#include "../Widget.h"

class GridActor;
struct Trap;

class GridMapPickerSelectionInfoWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

private:
	void DrawGridActorInfo();
	void DrawTrapNodeInfo();

public:
	GridActor* selectedGridActor = nullptr;
	
	Trap* selectedTrap = nullptr;
};

#pragma once

#include "../Widget.h"

class GridActor;

class GridMapPickerSelectionInfoWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

	GridActor* selectedGridActor = nullptr;
};

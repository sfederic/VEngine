#pragma once

#include "../Widget.h"

class GridActor;

class GridMapPickerSelectionInfo : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

	GridActor* selectedGridActor = nullptr;
};

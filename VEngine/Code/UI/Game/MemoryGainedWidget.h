#pragma once
#include "../Widget.h"

struct Memory;

//Notification to show when a Memory is gained during gameplay.
struct MemoryGainedWidget : Widget
{
	Memory* memoryToDisplay = nullptr;

	virtual void Draw(float deltaTime) override;
};

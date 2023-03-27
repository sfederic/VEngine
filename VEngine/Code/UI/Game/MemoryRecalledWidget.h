#pragma once
#include "../Widget.h"

struct Memory;

//Shows up when a Memory is "checked" to let the player know what's triggering new interacts/dialogue/etc.
struct MemoryRecalledWidget : Widget
{
	Memory* recalledMemory = nullptr;

	virtual void Draw(float deltaTime) override;
};

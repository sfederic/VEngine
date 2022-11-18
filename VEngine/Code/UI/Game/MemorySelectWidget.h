#pragma once

#include "../Widget.h"

//New memory Widget that shows 3 main memories the player can select or fuse to gain new memories.
class MemorySelectWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

private:
	int selectedMemoryIndex = -1;
};

#pragma once

#include "../Widget.h"

struct Memory;

//New memory Widget that shows 3 main memories the player can select or fuse to gain new memories.
class MemorySelectWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

private:
	void DrawMemories();
	void DrawMemoryMergeMenu();

	void MergeMemories();

	int selectedMemoryIndex = -1;

	std::vector<Memory*> memoriesToMerge;
	bool memoryMergeModeActive = false;
};

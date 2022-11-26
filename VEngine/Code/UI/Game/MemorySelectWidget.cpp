#include "vpch.h"
#include "MemorySelectWidget.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/Memory.h"
#include "Gameplay/Memories/MergedMemories/MemoryMerger.h"
#include "Log.h"

void MemorySelectWidget::Draw(float deltaTime)
{
	DrawMemoryMergeMenu();
	DrawMemories();
}

void MemorySelectWidget::DrawMemories()
{
	std::vector<Layout> layouts;
	layouts.push_back(PercentAlignLayout(0.45f, 0.3f, 0.55f, 0.5f));
	layouts.push_back(PercentAlignLayout(0.3f, 0.6f, 0.4f, 0.8f));
	layouts.push_back(PercentAlignLayout(0.6f, 0.6f, 0.7f, 0.8f));

	int layoutsIndex = 0;

	for (const auto& [name, memory] : GameInstance::playerMemories)
	{
		if (ImageButton(memory->imageFile, layouts[layoutsIndex]))
		{
			selectedMemoryIndex = layoutsIndex;

			if (memoryMergeModeActive)
			{
				memoriesToMerge.push_back(memory);
			}
		}

		Text(name, layouts[layoutsIndex]);

		layoutsIndex++;
	}
}

void MemorySelectWidget::DrawMemoryMergeMenu()
{
	if (Button("Merge Memories", PercentAlignLayout(0.4f, 0.1f, 0.6f, 0.2f)))
	{
		memoryMergeModeActive = !memoryMergeModeActive;
	}

	if (memoryMergeModeActive)
	{
		Text("Select Memories to merge", PercentAlignLayout(0.4f, 0.2f, 0.6f, 0.3f));

		if (Button("Merge", PercentAlignLayout(0.4f, 0.4f, 0.6f, 0.5f)))
		{
			MergeMemories();
		}
	}
}

void MemorySelectWidget::MergeMemories()
{
	if (memoriesToMerge.size() < 2)
	{
		Log("not enough memories selected to merge.");
		return;
	}
	if (memoriesToMerge.size() > 2)
	{
		Log("too many memories selected to merge.");
		return;
	}

	MemoryMerger::MergeMemories(memoriesToMerge[0]->name, memoriesToMerge[1]->name);
}

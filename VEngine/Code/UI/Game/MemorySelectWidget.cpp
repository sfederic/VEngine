#include "vpch.h"
#include "MemorySelectWidget.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/Memory.h"

void MemorySelectWidget::Draw(float deltaTime)
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
		}

		Text(name, layouts[layoutsIndex]);

		layoutsIndex++;
	}
}

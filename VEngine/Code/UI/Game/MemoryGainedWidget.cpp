#include "vpch.h"
#include "MemoryGainedWidget.h"
#include "Gameplay/Memory.h"

void MemoryGainedWidget::Draw(float deltaTime)
{
	Layout layout = PercentAlignLayout(0.f, 0.f, 1.f, 0.2f);

	FillRect(layout);
	Text(L"Memory Gained", layout);

	layout.AddVerticalSpace(30.f);
	Text(memoryToDisplay->name, layout);

	layout.AddVerticalSpace(30.f);
	Text(memoryToDisplay->description, layout);

	if (!memoryToDisplay->imageFile.empty())
	{
		auto imageLayout = PercentAlignLayout(0.1f, 0.2f, 0.9f, 0.9f);
		Image(memoryToDisplay->imageFile, imageLayout);
	}
}

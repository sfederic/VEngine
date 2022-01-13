#include "MemoryGainedWidget.h"
#include "Gameplay/Memory.h"
#include "VString.h"

void MemoryGainedWidget::Draw(float deltaTime)
{
	Layout layout = PercentAlignLayout(0.f, 0.f, 1.f, 0.1f);

	FillRect(layout);
	Text(L"Memory Gained", layout);

	layout.AddVerticalSpace(30.f);
	Text(VString::stows(memoryToDisplay->name), layout);
}

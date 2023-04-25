#include "vpch.h"
#include "MemoryRecalledWidget.h"
#include "Gameplay/Memory.h"
#include "Core/VString.h"

void MemoryRecalledWidget::Draw(float deltaTime)
{
	if (recalledMemory)
	{
		Layout layout = PercentAlignLayout(0.1f, 0.8f, 0.9f, 1.f);

		FillRect(layout);
		Text(L"Memory Recalled", layout);

		layout.AddVerticalSpace(30.f);
		Text(VString::stows(recalledMemory->name), layout);

		layout.AddVerticalSpace(30.f);
		Text(VString::stows(recalledMemory->description), layout);
	}
}

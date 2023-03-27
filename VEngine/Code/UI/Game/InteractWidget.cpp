#include "vpch.h"
#include "InteractWidget.h"
#include "Core/VString.h"

void InteractWidget::Draw(float deltaTime)
{
	auto rect = AlignLayout(150.f, 100.f, Align::Bottom);

	FillRect(rect, {0.7f, 0.7f, 0.7f, 1.f}, 0.7f);
	Text(interactText, rect);
}

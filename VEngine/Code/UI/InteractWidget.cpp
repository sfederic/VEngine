#include "InteractWidget.h"

void InteractWidget::Tick(float deltaTime)
{
	auto rect = AlignLayout(100.f, 100.f, Align::TopRight);

	Rect(rect);
	Text(interactText, rect);
}

#include "InteractWidget.h"

void InteractWidget::Tick(float deltaTime)
{
	auto rect = AlignLayout(200.f, 100.f, Align::Bottom);

	Rect(rect);
	Text(interactText, rect);
}

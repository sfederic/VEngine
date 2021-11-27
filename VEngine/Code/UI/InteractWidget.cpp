#include "InteractWidget.h"

void InteractWidget::Tick(float deltaTime)
{
	auto rect = AlignLayoutCenter(300.f, 150.f);

	Rect(rect);
	Text(interactText, rect);
}

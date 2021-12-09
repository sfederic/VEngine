#include "InteractWidget.h"
#include "VString.h"

void InteractWidget::Tick(float deltaTime)
{
	auto rect = AlignLayout(150.f, 50.f, Align::Bottom);

	FillRect(rect, {0.7f, 0.7f, 0.7f, 1.f}, 0.7f);
	Text(interactText, rect);
}

void InteractWidget::SetText(std::string text)
{
	interactText = VString::stows(text);
}

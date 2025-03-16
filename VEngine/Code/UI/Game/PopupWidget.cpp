
#include "PopupWidget.h"

void PopupWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	auto layout = PercentAlignLayout(0.3f, 0.3f, 0.7f, 0.7f);
	FillRect(layout);
	Text(popupText, layout);
}

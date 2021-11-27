#include "DialogueWidget.h"

void DialogueWidget::Tick(float deltaTime)
{
	int sx, sy;
	GetScreenSpaceCoords(sx, sy);

	Image("speech_bubble.png", (float)sx - 50.f, (float)sy - 50.f, 200.f, 200.f);
	Text(displayText, { (float)sx, (float)sy, (float)sx + 150.f, (float)sy + 150.f });
}

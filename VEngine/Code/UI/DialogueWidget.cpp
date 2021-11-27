#include "DialogueWidget.h"

void DialogueWidget::Tick(float deltaTime)
{
	//TODO: this whole function looks fucking wonky.

	int sx, sy;
	GetScreenSpaceCoords(sx, sy);

	sy -= 75.f;
	D2D1_RECT_F textRect = CenterLayoutOnScreenSpaceCoords(100.f, 100.f, sx, sy);

	D2D1_RECT_F imageRect = {
		textRect.left - 50.f, textRect.top - 50.f, textRect.right + 50.f, textRect.bottom + 50.f 
	};

	Image("speech_bubble.png", imageRect);
	Text(displayText, textRect);
}

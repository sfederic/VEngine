#include "vpch.h"
#include "DialogueWidget.h"
#include "Core/VString.h"

void DialogueWidget::Draw(float deltaTime)
{
	int sx, sy;
	GetScreenSpaceCoords(sx, sy);

	sy -= 200.f;
	Layout layout = CenterLayoutOnScreenSpaceCoords(100.f, 100.f, sx, sy);

	D2D1_RECT_F imageRect = {
		layout.rect.left - 50.f, layout.rect.top - 25.f, layout.rect.right + 50.f, layout.rect.bottom + 75.f
	};

	Image("UI/speech_bubble.png", imageRect);
	Text(dialogueText, layout);
}

void DialogueWidget::SetText(std::wstring text)
{
	dialogueText = text;
}

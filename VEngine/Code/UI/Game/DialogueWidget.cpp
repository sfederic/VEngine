
#include "DialogueWidget.h"

void DialogueWidget::Draw(float deltaTime)
{
	__super::Draw(deltaTime);

	ProgressCurrentText(deltaTime);

	int sx, sy;
	GetScreenSpaceCoords(sx, sy);

	//Push y-axis up so widget is just above actor
	sy -= 200.f;
	const Layout layout = CenterLayoutOnScreenSpaceCoords(100.f, 100.f, sx, sy);

	const D2D1_RECT_F imageRect = {
		layout.rect.left - 50.f, layout.rect.top - 25.f, layout.rect.right + 50.f, layout.rect.bottom + 75.f
	};

	Image("UI/speech_bubble.png", imageRect);

	Text(currentDialogueText, layout, Widget::TextAlign::Leading);

	//Check self removal after text has sat at its end for a moment.
	if (currentDialogueCharIndex >= fullDialogueText.size() - 1)
	{
		removeWidgetTimer += deltaTime;
		if (removeWidgetTimer > 2.f)
		{
			ResetAllValues();

			if (setToDeleteOnTextProgressEnd)
			{
				RemoveFromViewportAndDelete();
			}
			else
			{
				RemoveFromViewport();
			}
		}

		return;
	}
}

void DialogueWidget::ResetAllValues()
{
	removeWidgetTimer = 0.f;
	currentDialogueCharIndex = 0;
	fullDialogueText.clear();
	currentDialogueText.clear();
}

void DialogueWidget::ProgressCurrentText(float deltaTime)
{
	textProgressTimer += deltaTime;
	constexpr float textSpeed = 0.05f;
	if (textProgressTimer > textSpeed && (currentDialogueCharIndex < fullDialogueText.size()))
	{
		textProgressTimer = 0.f;
		currentDialogueText.push_back(fullDialogueText.at(currentDialogueCharIndex));
		currentDialogueCharIndex++;
	}
}

#include "vpch.h"
#include "DialogueWidget.h"
#include "Actors/Game/Player.h"

void DialogueWidget::Draw(float deltaTime)
{
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

	Text(currentDialogueText, layout);

	//Check self removal after text has sat at its end for a moment.
	if (currentDialogueCharIndex >= fullDialogueText.size() - 1)
	{
		removeWidgetTimer += deltaTime;
		if (removeWidgetTimer > 2.f)
		{
			removeWidgetTimer = 0.f;
			currentDialogueCharIndex = 0;
			fullDialogueText.clear();
			currentDialogueText.clear();
			//@Todo: this is no good. Need a PlayerQuickThoughtWidget else potential NPC dialogue wigdets
			//will trigger this.
			Player::system.GetOnlyActor()->SetQuickThoughtOff();
			this->RemoveFromViewport();
		}

		return;
	}
}

void DialogueWidget::ProgressCurrentText(float deltaTime)
{
	textProgressTimer += deltaTime;
	constexpr float textSpeed = 0.065f;
	if (textProgressTimer > textSpeed && (currentDialogueCharIndex < fullDialogueText.size() - 1))
	{
		textProgressTimer = 0.f;
		currentDialogueText.push_back(fullDialogueText.at(currentDialogueCharIndex));
		currentDialogueCharIndex++;
	}
}

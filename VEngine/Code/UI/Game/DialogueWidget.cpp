#include "vpch.h"
#include "DialogueWidget.h"

void DialogueWidget::Draw(float deltaTime)
{
	Layout layout = PercentAlignLayout(0.1f, 0.6f, 0.9f, 0.9f);
	FillRect(layout);
	Text(speakerName, layout, TextAlign::Justified);
	layout.AddVerticalSpace(30.f);
	Text(dialogueText, layout, TextAlign::Justified);
}

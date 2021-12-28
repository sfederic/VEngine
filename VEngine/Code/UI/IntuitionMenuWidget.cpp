#include "IntuitionMenuWidget.h"
#include "Render/Renderer.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/Intuition.h"
#include "VString.h"

void IntuitionMenuWidget::Tick(float deltaTime)
{
	D2D1_RECT_F rect = PercentAlignLayout(0.25f, 0.25f, 0.75f, 0.75f);
	FillRect(rect);

	//Set text rect to begining of layoutrect, then increment in forloop
	D2D1_RECT_F textRect = rect;
	textRect.bottom = rect.top;

	for (auto& intuitionPair : GameInstance::playerIntuitions)
	{
		Intuition* intuition = intuitionPair.second;

		Text(VString::stows(intuition->name), textRect);
		textRect.top += 30.f;
		textRect.bottom += 30.f;

		Text(VString::stows(intuition->description), textRect);
		textRect.top += 30.f;
		textRect.bottom += 30.f;
	}
}

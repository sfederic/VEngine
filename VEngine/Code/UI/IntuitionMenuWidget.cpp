#include "IntuitionMenuWidget.h"
#include "Render/Renderer.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/Intuition.h"
#include "Actors/Game/Player.h"
#include "VString.h"

void IntuitionMenuWidget::Tick(float deltaTime)
{
	D2D1_RECT_F rect = AlignLayout(200.f, 200.f, Align::Center);
	FillRect(rect);

	//Set text rect to begining of layoutrect, then increment in forloop
	D2D1_RECT_F textRect = rect;
	textRect.bottom = rect.top;

	auto player = GameUtils::GetPlayer();
	for (auto& intuitionPair : player->intuitions)
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

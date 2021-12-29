#include "IntuitionMenuWidget.h"
#include "Render/Renderer.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/Intuition.h"
#include "VString.h"

void IntuitionMenuWidget::Draw()
{
	Layout layout = PercentAlignLayout(0.25f, 0.25f, 0.75f, 0.75f);
	FillRect(layout);

	//Set text rect to begining of layoutrect, then increment in forloop
	Layout textLayout = layout;
	textLayout.rect.bottom = textLayout.rect.top;

	for (auto& intuitionPair : GameInstance::playerIntuitions)
	{
		Intuition& intuition = intuitionPair.second;

		Text(VString::stows(intuition.name), textLayout);
		textLayout.AddVerticalSpace(30.f);

		Text(VString::stows(intuition.description), textLayout);
		textLayout.AddVerticalSpace(30.f);
	}
}

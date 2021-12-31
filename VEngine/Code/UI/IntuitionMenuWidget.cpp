#include "IntuitionMenuWidget.h"
#include "Render/Renderer.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/Intuition.h"
#include "VString.h"

void IntuitionMenuWidget::Draw()
{
	Layout layout = PercentAlignLayout(0.1f, 0.1f, 0.9f, 0.9f);
	FillRect(layout, {0.5f, 0.5f, 0.5f, 0.75f});

	//Set text rect to begining of layoutrect, then increment in forloop
	Layout textLayout = layout;
	textLayout.PushToTop();

	for (auto& intuitionPair : GameInstance::playerIntuitions)
	{
		Intuition& intuition = intuitionPair.second;

		textLayout.AddVerticalSpace(30.f);
		Text(VString::stows(intuition.name), textLayout);

		textLayout.AddVerticalSpace(30.f);
		Text(VString::stows(intuition.description), textLayout);
	}
}

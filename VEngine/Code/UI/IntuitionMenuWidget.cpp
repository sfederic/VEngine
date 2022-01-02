#include "IntuitionMenuWidget.h"
#include "Render/Renderer.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/Intuition.h"
#include "VString.h"

void IntuitionMenuWidget::Draw(float deltaTime)
{
	//Intuiton select scroll menu
	{
		Layout layout = PercentAlignLayout(0.1f, 0.1f, 0.4f, 0.9f);
		FillRect(layout, { 0.5f, 0.5f, 0.5f, 0.75f });

		//Set text rect to begining of layoutrect, then increment in forloop
		Layout textLayout = layout;
		textLayout.PushToTop();
		textLayout.rect.bottom += 30.f;

		Text(L"Intuitions", textLayout);

		for (auto& intuitionPair : GameInstance::playerIntuitions)
		{
			Intuition& intuition = intuitionPair.second;

			textLayout.AddVerticalSpace(30.f);
			if (Button(VString::stows(intuition.name), textLayout))
			{
				selectedIntuition = &intuition;
			}
		}
	}

	//Selected intuition details
	{
		if (selectedIntuition)
		{
			Layout layout = PercentAlignLayout(0.5f, 0.1f, 0.9f, 0.9f);
			FillRect(layout, { 0.5f, 0.5f, 0.5f, 0.75f });

			Text(VString::wformat(L"Name: %S", selectedIntuition->name.c_str()), layout, TextAlign::Leading);
			layout.AddVerticalSpace(30.f);

			Text(VString::wformat(L"Desc: %S", selectedIntuition->description.c_str()), layout, TextAlign::Leading);
			layout.AddVerticalSpace(90.f);

			Text(VString::wformat(L"Actor GET: %S", selectedIntuition->actorAquiredFrom.c_str()), layout, TextAlign::Leading);
			layout.AddVerticalSpace(30.f);

			Text(VString::wformat(L"Hour GET: %d", selectedIntuition->hourAquired), layout, TextAlign::Leading);
			layout.AddVerticalSpace(30.f);

			Text(VString::wformat(L"Minute GET: %d", selectedIntuition->minuteAquired), layout, TextAlign::Leading);
			layout.AddVerticalSpace(30.f);

			Text(VString::wformat(L"World GET: %S", selectedIntuition->worldAquiredFrom.c_str()), layout, TextAlign::Leading);
			layout.AddVerticalSpace(30.f);
		}
	}
}

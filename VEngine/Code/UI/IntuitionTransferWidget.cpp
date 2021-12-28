#include "IntuitionTransferWidget.h"
#include "Gameplay/GameInstance.h"
#include "VString.h"
#include "Gameplay/Intuition.h"
#include "Gameplay/GameUtils.h"

void IntuitionTransferWidget::Tick(float deltaTime)
{
	//Display intuitons to select
	{
		D2D1_RECT_F rect = AlignLayout(200.f, 200.f, Align::Left);
		FillRect(rect);

		Text(L"Select Intuitions to carry over.", rect);

		D2D1_RECT_F itemRect = rect;
		itemRect.bottom = rect.top + 30.f;

		itemRect.top += 30.f;
		itemRect.bottom += 30.f;

		for (auto& intuitionPair : GameInstance::playerIntuitions)
		{
			Intuition& intuition = intuitionPair.second;

			itemRect.top += 30.f;
			itemRect.bottom += 30.f;

			//Handle selecting intuitions
			if (Button(VString::stows(intuition.name), itemRect))
			{
				selectedIntuitionsToTransfer.emplace(intuition.name, intuition);
			}

			itemRect.top += 30.f;
			itemRect.bottom += 30.f;

			Text(VString::stows(intuition.description), itemRect);
		}
	}

	//Display selected intuitions
	{
		D2D1_RECT_F layout = AlignLayout(200.f, 30.f, Align::Right);
		FillRect(layout);

		for (auto& intuitionPair : selectedIntuitionsToTransfer)
		{
			Intuition& intuition = intuitionPair.second;

			Text(VString::stows(intuition.name), layout);
			layout.top += 30.f;
			layout.bottom += 30.f;
		}

		if (Button(L"Reconnect", layout))
		{
			//Transfer selected intuitions
			GameInstance::DeletePlayerIntuitions();
			GameInstance::playerIntuitions = selectedIntuitionsToTransfer;

			GameUtils::LoadWorld("church_hall.vmap");
		}
	}
}

#include "IntuitionTransferWidget.h"
#include "Gameplay/GameInstance.h"
#include "VString.h"
#include "Gameplay/Intuition.h"
#include "Gameplay/GameUtils.h"

void IntuitionTransferWidget::Draw(float deltaTime)
{
	//Display intuitons to select
	{
		Layout layout = AlignLayout(200.f, 200.f, Align::Left);
		FillRect(layout);

		Text(L"Select Intuitions to carry over.", layout);

		Layout itemLayout = layout;
		itemLayout.rect.bottom = itemLayout.rect.top + 30.f;

		itemLayout.AddVerticalSpace(30.f);

		for (auto& intuitionPair : GameInstance::playerIntuitions)
		{
			Intuition& intuition = intuitionPair.second;

			itemLayout.AddVerticalSpace(30.f);

			//Handle selecting intuitions
			if (Button(VString::stows(intuition.name), itemLayout))
			{
				selectedIntuitionsToTransfer.emplace(intuition.name, intuition);
			}

			itemLayout.AddVerticalSpace(30.f);

			Text(VString::stows(intuition.description), itemLayout);
		}
	}

	//Display selected intuitions
	{
		Layout layout = AlignLayout(200.f, 30.f, Align::Right);
		FillRect(layout);

		for (auto& intuitionPair : selectedIntuitionsToTransfer)
		{
			Intuition& intuition = intuitionPair.second;

			Text(VString::stows(intuition.name), layout);
			layout.AddVerticalSpace(30.f);
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

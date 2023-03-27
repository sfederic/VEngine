#include "vpch.h"
#include "MemoryTransferWidget.h"
#include "Gameplay/GameInstance.h"
#include "Core/VString.h"
#include "Gameplay/Memory.h"
#include "Gameplay/GameUtils.h"

void MemoryTransferWidget::Draw(float deltaTime)
{
	//Display intuitons to select
	{
		Layout layout = AlignLayout(200.f, 200.f, Align::Left);
		FillRect(layout);

		Text(L"Select Memories to carry over.", layout);

		Layout itemLayout = layout;
		itemLayout.rect.bottom = itemLayout.rect.top + 30.f;

		itemLayout.AddVerticalSpace(30.f);

		for (auto& memoryPair : GameInstance::playerMemories)
		{
			auto& memory = memoryPair.second;

			itemLayout.AddVerticalSpace(30.f);

			//Handle selecting memories
			if (Button(VString::stows(memory->name), itemLayout))
			{
				selectedMemoriesToTransfer.emplace(memory->name, memory);
			}

			//itemLayout.AddVerticalSpace(30.f);
			//Text(VString::stows(memory.description), itemLayout);
		}
	}

	//Display selected memories
	{
		Layout layout = AlignLayout(200.f, 30.f, Align::Right);

		FillRect(layout);

		for (auto& memoryPair : selectedMemoriesToTransfer)
		{
			auto memory = memoryPair.second;

			Text(VString::stows(memory->name), layout);
			layout.AddVerticalSpace(30.f);
		}

		if (Button(L"Reconnect", layout))
		{
			//Transfer selected memories
			//GameInstance::DeletePlayerMemories();
			GameInstance::playerMemories = selectedMemoriesToTransfer;

			//GameUtils::LoadWorld("church.vmap");
		}
	}
}

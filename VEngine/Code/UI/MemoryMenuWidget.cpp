#include "MemoryMenuWidget.h"
#include "Render/Renderer.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/Memory.h"
#include "Actors/Game/Player.h"
#include "VString.h"

void MemoryMenuWidget::Draw(float deltaTime)
{
	//Intuiton select scroll menu
	{
		Layout layout = PercentAlignLayout(0.1f, 0.1f, 0.4f, 0.9f);
		FillRect(layout, { 0.5f, 0.5f, 0.5f, 0.75f });

		//Set text rect to begining of layoutrect, then increment in forloop
		Layout textLayout = layout;
		textLayout.PushToTop();
		textLayout.rect.bottom += 30.f;

		Text(L"Memories", textLayout);

		for (auto& memoryPair : GameInstance::playerMemories)
		{
			auto memory = memoryPair.second;

			textLayout.AddVerticalSpace(30.f);
			if (Button(VString::stows(memory->name), textLayout))
			{
				selectedMemory = memory;

				GameUtils::GetPlayer()->memoryNameToSpawn = selectedMemory->name;
			}
		}
	}

	//Selected memory details
	{
		if (selectedMemory)
		{
			Layout layout = PercentAlignLayout(0.5f, 0.1f, 0.9f, 0.5f);
			FillRect(layout, { 0.5f, 0.5f, 0.5f, 0.75f });

			layout = PercentAlignLayout(0.5f, 0.1f, 0.9f, 0.2f);

			Text(VString::wformat(L"Name: %S", selectedMemory->name.c_str()), layout, TextAlign::Leading);
			layout.AddVerticalSpace(30.f);

			Text(VString::wformat(L"Desc: %S", selectedMemory->description.c_str()), layout, TextAlign::Leading);
			layout.AddVerticalSpace(90.f);

			Text(VString::wformat(L"Actor GET: %S", selectedMemory->actorAquiredFrom.c_str()), layout, TextAlign::Leading);
			layout.AddVerticalSpace(30.f);

			Text(VString::wformat(L"Hour GET: %d", selectedMemory->hourAquired), layout, TextAlign::Leading);
			layout.AddVerticalSpace(30.f);

			Text(VString::wformat(L"Minute GET: %d", selectedMemory->minuteAquired), layout, TextAlign::Leading);
			layout.AddVerticalSpace(30.f);

			Text(VString::wformat(L"World GET: %S", selectedMemory->worldAquiredFrom.c_str()), layout, TextAlign::Leading);
			layout.AddVerticalSpace(30.f);

			layout = PercentAlignLayout(0.5f, 0.5f, 0.9f, 0.9f);
			Image(selectedMemory->imageFile, layout);
		}
	}
}

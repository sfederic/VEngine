#include "vpch.h"
#include "MemoryMenuWidget.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/Memory.h"
#include "Actors/Game/Player.h"
#include "Core/VString.h"
#include "Core/Input.h"
#include "Core/Core.h"

void MemoryMenuWidget::Draw(float deltaTime)
{
	if (Core::gameWorldPaused)
	{
		if (Input::GetKeyDown(Keys::Enter)) //Sneaky 'GetKeyDown' so that it works with player's input
		{
			RemoveFromViewport();
			GameUtils::PlayAudioOneShot("cursor.wav");
			Core::gameWorldPaused = false;
		}
	}

	//Memory select scroll menu
	{
		Layout layout = PercentAlignLayout(0.1f, 0.1f, 0.4f, 0.9f);
		FillRect(layout, { 0.5f, 0.5f, 0.5f, 0.75f });

		//Set text rect to begining of layoutrect, then increment in forloop
		Layout textLayout = layout;
		textLayout.PushToTop();
		textLayout.rect.bottom += 30.f;

		selectedMemoryHighlightLayout = textLayout;

		Text(L"Memories", textLayout);

		std::vector<Memory*> memories;

		for (auto& [name, memory] : GameInstance::playerMemories)
		{
			memories.push_back(memory);
			textLayout.AddVerticalSpace(30.f);
			Text(name, textLayout);
		}

		//Show selected memory highlight

		if (!memories.empty())
		{
			if (memories.size() == 1)
			{
				selectedMemory = memories.at(0);
			}
			else if (Input::GetKeyUp(Keys::S))
			{
				if (selectedMemoryIndex < memories.size() - 1)
				{
					selectedMemoryIndex++;
					selectedMemory = memories.at(selectedMemoryIndex);
					GameUtils::PlayAudioOneShot("cursor.wav");
				}
			}
			else if (Input::GetKeyUp(Keys::W))
			{
				if (selectedMemoryIndex > 0)
				{
					selectedMemoryIndex--;
					selectedMemory = memories.at(selectedMemoryIndex);
					GameUtils::PlayAudioOneShot("cursor.wav");
				}
			}

			selectedMemoryHighlightLayout.AddVerticalSpace(30.f * (selectedMemoryIndex + 1)); //'+1' for the header
			Rect(selectedMemoryHighlightLayout);
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

			Text(VString::wformat(L"Actor GET: %S", selectedMemory->actorAcquiredFrom.c_str()), layout, TextAlign::Leading);
			layout.AddVerticalSpace(30.f);

			Text(VString::wformat(L"World GET: %S", selectedMemory->worldAcquiredFrom.c_str()), layout, TextAlign::Leading);
			layout.AddVerticalSpace(30.f);

			layout = PercentAlignLayout(0.5f, 0.5f, 0.9f, 0.9f);
			Image(selectedMemory->imageFile, layout);
		}
	}
}

#include "Console.h"
#include "Input.h"
#include "UISystem.h"
#include "CoreSystem.h"
#include "RenderSystem.h"
#include <vector>
#include "FileSystem.h"
#include "World.h"
#include "DebugMenu.h"
#include "WorldEditor.h"
#include "Profiler.h"

Console gConsole;

namespace ExecuteStrings
{
	const wchar_t* GPU = L"GPU";
	const wchar_t* FPS = L"FPS";
	const wchar_t* SNAP = L"SNAP";
	const wchar_t* PROFILE = L"PROFILE";
	const wchar_t* ACTOR = L"ACTOR";
}

void Console::ConsoleInput()
{
	if (gInputSystem.GetAnyKeyDown())
	{
		if (gInputSystem.GetKeyDownState(Keys::BackSpace) && !consoleString.empty())
		{
			consoleString.pop_back();
		}
		else if (gInputSystem.currentDownKey != 0)
		{
			consoleString.push_back(gInputSystem.currentDownKey);
		}
	}
}

//Make sure D2D render target calls have been made (Begin/End Draw)
void Console::Tick()
{
	if (gInputSystem.GetKeyUpState(Keys::Tilde)) //~ key, like doom and unreal
	{
		bConsoleActive = !bConsoleActive;
		return;
	}

	if (bConsoleActive)
	{
		if (gInputSystem.GetKeyUpState(Keys::Enter))
		{
			consoleString.pop_back(); //Remove '\r' return carriage
			Console::ExecuteString();
			bConsoleActive = false;
			return;
		}

		Console::ConsoleInput();

		float width = (float)gCoreSystem.windowWidth;
		float height = (float)gCoreSystem.windowHeight;

		gUISystem.d2dRenderTarget->DrawRectangle({ 0, height - 24.f, width, height }, gUISystem.brushText);

		gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		gUISystem.d2dRenderTarget->DrawText(consoleString.c_str(), consoleString.size(), gUISystem.textFormat,
			{ 0, height - 20.f, width, height }, gUISystem.brushText);
	}
}

//Execute values need to be uppercase with WndProc
void Console::ExecuteString()
{
	if (consoleString == ExecuteStrings::GPU)
	{
		gDebugMenu.bGPUMenuOpen = !gDebugMenu.bGPUMenuOpen;
	}
	else if (consoleString == ExecuteStrings::SNAP)
	{
		gDebugMenu.bSnapMenuOpen = !gDebugMenu.bSnapMenuOpen;
	}
	else if (consoleString == ExecuteStrings::PROFILE)
	{
		gDebugMenu.bProfileMenuOpen = !gDebugMenu.bProfileMenuOpen;
	}
	else if (consoleString == ExecuteStrings::FPS)
	{
		gDebugMenu.bFPSMenuOpen = !gDebugMenu.bFPSMenuOpen;
	}
	else if (consoleString == ExecuteStrings::ACTOR)
	{
		gDebugMenu.bActorStatsMenuOpen = !gDebugMenu.bActorStatsMenuOpen;
	}
	else
	{
		gDebugMenu.AddNotification(L"No command found");
	}


	consoleString.clear();
}

void Console::DrawViewItems()
{
	for (int i = 0; i < viewItems.size(); i++)
	{
		const float yMarginIncrement = 20.f * i;

		const float width = (float)gCoreSystem.windowWidth;
		const float height = (float)gCoreSystem.windowHeight;

		gUISystem.d2dRenderTarget->DrawTextA(viewItems[i].text, wcslen(viewItems[i].text), gUISystem.textFormat,
			{ 0, yMarginIncrement, width, height }, gUISystem.brushText);
	}
}

#include "Console.h"
#include <dwrite.h>
#include <d2d1.h>
#include "Input.h"
#include "UI/UISystem.h"
#include "DebugMenu.h"
#include "Render/Renderer.h"

Console console;

Console::Console()
{
	//Execute values need to be uppercase with WndProc
	executeMap.emplace(L"SNAP", []() { debugMenu.snapMenuOpen = !debugMenu.snapMenuOpen; });
	executeMap.emplace(L"PROFILE", []() { debugMenu.profileMenuOpen = !debugMenu.profileMenuOpen; });
	executeMap.emplace(L"FPS", []() { debugMenu.fpsMenuOpen = !debugMenu.fpsMenuOpen; });
	executeMap.emplace(L"PROPS", []() { debugMenu.propsMenuOpen = !debugMenu.propsMenuOpen; });
	executeMap.emplace(L"COMMANDS", []() { debugMenu.commandsMenuOpen = !debugMenu.commandsMenuOpen; });
	executeMap.emplace(L"GPU", []() { debugMenu.gpuMenuOpen = !debugMenu.gpuMenuOpen; });
	executeMap.emplace(L"ACTOR", []() { debugMenu.actorInspectMenuOpen = !debugMenu.actorInspectMenuOpen; });
	executeMap.emplace(L"STATS", []() { debugMenu.worldStatsMenuOpen = !debugMenu.worldStatsMenuOpen; });
}

void Console::ConsoleInput()
{
	if (Input::GetAnyKeyDown())
	{
		if (Input::GetKeyDown(Keys::BackSpace) && !consoleString.empty())
		{
			consoleString.pop_back();
		}
		else if (Input::currentDownKeys.size() > 0)
		{
			consoleString.push_back((int)Input::currentDownKeys.back());
		}
	}
}

//Make sure D2D render target calls have been made (Begin/End Draw)
void Console::Tick()
{
	if (Input::GetKeyUp(Keys::Tilde)) //~ key, like doom and unreal
	{
		bConsoleActive = !bConsoleActive;
		return;
	}

	if (bConsoleActive)
	{
		if (Input::GetKeyUp(Keys::Enter))
		{
			consoleString.pop_back(); //Remove '\r' return carriage
			ExecuteString();
			bConsoleActive = false;
			return;
		}

		ConsoleInput();

		float width = (float)renderer.GetViewportWidth();
		float height = (float)renderer.GetViewportHeight();

		uiSystem.d2dRenderTarget->DrawRectangle({ 0, height - 50.f, width, height }, uiSystem.brushText);

		uiSystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		uiSystem.d2dRenderTarget->DrawText(consoleString.c_str(), consoleString.size(), uiSystem.textFormat,
			{ 0, height - 50.f, width, height }, uiSystem.brushText);
	}
}

void Console::ExecuteString()
{
	auto executeIt = executeMap.find(consoleString);
	if (executeIt != executeMap.end())
	{
		executeIt->second();
	}
	else
	{
		debugMenu.AddNotification(L"No command found");
	}

	consoleString.clear();
}

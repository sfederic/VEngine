#include "Console.h"
#include <dwrite.h>
#include <d2d1.h>
#include "Input.h"
#include "UI/UISystem.h"
#include "DebugMenu.h"
#include "Render/Renderer.h"

Console console;

namespace ExecuteStrings
{
	const wchar_t* GPU = L"GPU";
	const wchar_t* FPS = L"FPS";
	const wchar_t* SNAP = L"SNAP";
	const wchar_t* PROFILE = L"PROFILE";
	const wchar_t* ACTOR = L"ACTOR";
	const wchar_t* PROPS = L"PROPS";
	const wchar_t* INSPECT = L"INSPECT";
	const wchar_t* COMMANDS = L"COMMANDS";
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

//Execute values need to be uppercase with WndProc
void Console::ExecuteString()
{
	/*if (consoleString == ExecuteStrings::GPU)
	{
		debugMenu.bGPUMenuOpen = !debugMenu.bGPUMenuOpen;
	}*/
	if (consoleString == ExecuteStrings::SNAP)
	{
		debugMenu.snapMenuOpen = !debugMenu.snapMenuOpen;
	}
	else if (consoleString == ExecuteStrings::PROFILE)
	{
		debugMenu.profileMenuOpen = !debugMenu.profileMenuOpen;
	}
	else if (consoleString == ExecuteStrings::FPS)
	{
		debugMenu.fpsMenuOpen = !debugMenu.fpsMenuOpen;
	}
	/*else if (consoleString == ExecuteStrings::ACTOR)
	{
		debugMenu.bActorStatsMenuOpen = !debugMenu.bActorStatsMenuOpen;
	}
	else if (consoleString == ExecuteStrings::INSPECT)
	{
		debugMenu.bActorInspectMenuOpen = !debugMenu.bActorInspectMenuOpen;
	}*/
	else if (consoleString == ExecuteStrings::PROPS)
	{
		debugMenu.propsMenuOpen = !debugMenu.propsMenuOpen;
	}	
	else if (consoleString == ExecuteStrings::COMMANDS)
	{
		debugMenu.commandsMenuOpen = !debugMenu.commandsMenuOpen;
	}
	else
	{
		debugMenu.AddNotification(L"No command found");
	}

	consoleString.clear();
}

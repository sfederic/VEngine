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
	const wchar_t* EXIT = L"EXIT";
	const wchar_t* GPU = L"GPU";
	const wchar_t* D3D_TIME = L"D3D TIME";
	const wchar_t* POP = L"POP";
	const wchar_t* CLEAR = L"CLEAR";
	const wchar_t* LEVEL = L"LEVEL";
	const wchar_t* SNAP = L"SNAP";
	const wchar_t* PROFILE = L"PROFILE";
}

namespace LevelNames
{
	const wchar_t* TestLevel = L"TESTLEVEL";
}

void Console::ConsoleInput()
{
	if (gInputSystem.GetAnyKeyDown() && gInputSystem.currentDownKey != VK_OEM_3)
	{
		if (gInputSystem.GetKeyDownState(VK_BACK) && consoleStringIndex > 0)
		{
			consoleStringIndex--;
		}
		else if ((consoleStringIndex < _countof(consoleString)) && (gInputSystem.currentDownKey != 0))
		{
			consoleString[consoleStringIndex] = gInputSystem.currentDownKey;
			consoleStringIndex++;
		}
	}
}

//Make sure D2D render target calls have been made (Begin/End Draw)
void Console::Tick()
{
	if (bConsoleActive)
	{
		Console::ConsoleInput();

		float width = (float)gCoreSystem.windowWidth;
		float height = (float)gCoreSystem.windowHeight;

		gUISystem.d2dRenderTarget->DrawRectangle({ 0, height - 20.f, width, height }, gUISystem.brushTransparentMenu);

		gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		gUISystem.d2dRenderTarget->DrawText(consoleString, consoleStringIndex, gUISystem.textFormat,
			{ 0, height - 20.f, width, height }, gUISystem.brushText);
	}

	if (gInputSystem.GetKeyUpState(VK_OEM_3)) //~ key, like doom and unreal
	{
		bConsoleActive = !bConsoleActive;
	}
	else if (gInputSystem.GetKeyDownState(VK_RETURN) && bConsoleActive)
	{
		Console::ExecuteString();
	}
}

//Execute values need to be uppercase with WndProc
void Console::ExecuteString()
{
	ConsoleViewItem item = {};

	if (wcsncmp(consoleString, ExecuteStrings::EXIT, wcslen(ExecuteStrings::EXIT)) == 0)
	{
		gCoreSystem.msg.message = WM_QUIT;
	}
	else if (wcsncmp(consoleString, ExecuteStrings::GPU, wcslen(ExecuteStrings::GPU)) == 0)
	{
		//_snwprintf_s(item.text, sizeof(item.text), L"%ls", gRenderSystem.adaptersDesc[0].Description);
		viewItems.push_back(item);
	}
	else if (wcsncmp(consoleString, ExecuteStrings::D3D_TIME, wcslen(ExecuteStrings::D3D_TIME)) == 0)
	{
		//_snwprintf_s(item.text, sizeof(item.text), L"D3D11 Render: %f", gRenderSystem.renderTime);
		viewItems.push_back(item);
	}
	else if (wcsncmp(consoleString, ExecuteStrings::POP, wcslen(ExecuteStrings::POP)) == 0)
	{
		if (viewItems.size() > 0)
		{
			viewItems.pop_back();
		}
	}
	else if (wcsncmp(consoleString, ExecuteStrings::CLEAR, wcslen(ExecuteStrings::CLEAR)) == 0) //CLEAR TEXT
	{
		viewItems.clear();
	}
	else if(wcsncmp(consoleString, LevelNames::TestLevel, wcslen(LevelNames::TestLevel)) == 0) //LEVEL LOADING
	{
		gFileSystem.ReadAllActorSystems(GetWorld(), "LevelSaves/test.sav");
	}
	else if (wcsncmp(consoleString, ExecuteStrings::SNAP, wcslen(ExecuteStrings::SNAP)) == 0) //Toggle grid snap
	{
		gWorldEditor.bMoveActorsInIncrements = !gWorldEditor.bMoveActorsInIncrements;

		if (gWorldEditor.bMoveActorsInIncrements)
		{
			debugMenu.AddNotification(L"SNAP ON");
		}
		else if (!gWorldEditor.bMoveActorsInIncrements)
		{
			debugMenu.AddNotification(L"SNAP OFF");
		}
	}
	else if (wcsncmp(consoleString, ExecuteStrings::PROFILE, wcslen(ExecuteStrings::PROFILE)) == 0) //Show profile stats
	{
		for (int i = 0; i < gProfiler.timeFrames.size(); i++)
		{
			TimeFrame timeFrame = gProfiler.timeFrames[i];
			_snwprintf_s(item.text, sizeof(item.text), L"Profile function: %ls", std::to_wstring(timeFrame.GetAverageTime()));
			viewItems.push_back(item);
		}
	}
	else
	{
		debugMenu.notifications.push_back(DebugNotification(L"No command found."));
	}

	//Reset console string and index
	memset(consoleString, 0, _countof(consoleString) * sizeof(wchar_t));
	consoleStringIndex = 0;
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

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
}

namespace LevelNames
{
	const wchar_t* TestLevel = L"TESTLEVEL";
}

void Console::ConsoleInput()
{
	if (gInputSystem.GetAnyKeyDown() && gInputSystem.currentDownKey != VK_OEM_3)
	{
		if (gInputSystem.GetKeyDownState(VK_BACK))
		{
			consoleString.pop_back();
			gUISystem.textLayout->Release();
			gUISystem.writeFactory->CreateTextLayout(consoleString.c_str(), consoleString.size(),
				gUISystem.textFormat, gCoreSystem.windowWidth, gCoreSystem.windowHeight, &gUISystem.textLayout);
		}
		else if ((gInputSystem.currentDownKey != 0))
		{
			consoleString.push_back(gInputSystem.currentDownKey);
			gUISystem.textLayout->Release();
			gUISystem.writeFactory->CreateTextLayout(consoleString.c_str(), consoleString.size(),
				gUISystem.textFormat, gCoreSystem.windowWidth, gCoreSystem.windowHeight, &gUISystem.textLayout);
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

		//Old rendering 
		//gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		//gUISystem.d2dRenderTarget->DrawText(consoleString, consoleStringIndex, gUISystem.textFormat,
		//	{ 0, height - 20.f, width, height }, gUISystem.brushText);

		//New render with caret and textlayout
		gUISystem.textLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		gUISystem.d2dRenderTarget->DrawTextLayout({ 0, height - 20.f }, gUISystem.textLayout, gUISystem.brushText);
		DWRITE_TEXT_METRICS metrics = {};
		gUISystem.textLayout->GetMetrics(&metrics);
		gUISystem.d2dRenderTarget->FillRectangle({0 + metrics.width, height - 20.f, 
			0 + metrics.width + 2.f, height - 20.f + metrics.height }, gUISystem.brushText);
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
	
	//TODO: fix the compares here (move to std::wstring)
	if (wcsncmp(consoleString.c_str(), ExecuteStrings::EXIT, wcslen(ExecuteStrings::EXIT)) == 0)
	{
		gCoreSystem.msg.message = WM_QUIT;
	}
	else if (wcsncmp(consoleString.c_str(), ExecuteStrings::GPU, wcslen(ExecuteStrings::GPU)) == 0)
	{
		_snwprintf_s(item.text, sizeof(item.text), L"%ls", gRenderSystem.adaptersDesc[0].Description);
		viewItems.push_back(item);
	}
	else if (wcsncmp(consoleString.c_str(), ExecuteStrings::D3D_TIME, wcslen(ExecuteStrings::D3D_TIME)) == 0)
	{
		_snwprintf_s(item.text, sizeof(item.text), L"D3D11 Render: %f", gRenderSystem.renderTime);
		viewItems.push_back(item);
	}
	else if (wcsncmp(consoleString.c_str(), ExecuteStrings::POP, wcslen(ExecuteStrings::POP)) == 0)
	{
		if (viewItems.size() > 0)
		{
			viewItems.pop_back();
		}
	}
	else if (wcsncmp(consoleString.c_str(), ExecuteStrings::CLEAR, wcslen(ExecuteStrings::CLEAR)) == 0) //CLEAR TEXT
	{
		viewItems.clear();
	}
	else if(wcsncmp(consoleString.c_str(), LevelNames::TestLevel, wcslen(LevelNames::TestLevel)) == 0) //LEVEL LOADING
	{
		gFileSystem.ReadAllActorSystems(GetWorld(), "LevelSaves/test.sav");
	}
	else if (wcsncmp(consoleString.c_str(), ExecuteStrings::SNAP, wcslen(ExecuteStrings::SNAP)) == 0) //Toggle grid snap
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
	else
	{
		debugMenu.notifications.push_back(DebugNotification(L"No command found."));
	}

	//Reset console string and index
	consoleString.empty();
	gUISystem.textLayout->Release();
	gUISystem.writeFactory->CreateTextLayout(L"", 0, gUISystem.textFormat, gCoreSystem.windowWidth, gCoreSystem.windowHeight, &gUISystem.textLayout);
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

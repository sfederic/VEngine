#include "Console.h"
#include "Input.h"
#include "UIContext.h"
#include "Win32Util.h"
#include "DXUtil.h"
#include <vector>

wchar_t consoleString[128];
int consoleStringIndex = 0;
bool bConsoleActive = false;

std::vector<ConsoleViewItem> viewItems;

namespace ExecuteStrings
{
	const wchar_t* EXIT = L"EXIT";
	const wchar_t* GPU = L"GPU";
	const wchar_t* D3D_TIME = L"D3D TIME";
	const wchar_t* POP = L"POP";
	const wchar_t* CLEAR = L"CLEAR";
}

void Console::ConsoleInput()
{
	if (GetAnyKeyDown() && currentDownKey != VK_OEM_3)
	{
		if (GetKeyDownState(VK_BACK) && consoleStringIndex > 0)
		{
			consoleStringIndex--;
		}
		else if ((consoleStringIndex < _countof(consoleString)) && (currentDownKey != 0))
		{
			consoleString[consoleStringIndex] = currentDownKey; //wchar_t to int64 cast works
			consoleStringIndex++;
		}
	}
}

//Make sure D2D render target calls have been made (Begin/End Draw)
void Console::Tick(UIContext* ui, DXUtil* dx)
{
	if (bConsoleActive)
	{
		Console::ConsoleInput();

		ui->d2dRenderTarget->DrawRectangle({ 0, (float)windowHeight - 20.f, (float)windowWidth, (float)windowHeight }, ui->brush);
		ui->d2dRenderTarget->DrawText(consoleString, consoleStringIndex, ui->textFormat, { 0, (float)windowHeight - 20.f, 
			(float)windowWidth, (float)windowHeight }, ui->brush);
	}

	if (GetKeyUpState(VK_OEM_3)) //~ key, like doom and unreal
	{
		bConsoleActive = !bConsoleActive;
	}
	else if (GetKeyDownState(VK_RETURN) && bConsoleActive)
	{
		Console::ExecuteString(dx);
	}
}

//Execute values need to be uppercase with WndProc
void Console::ExecuteString(DXUtil* dx)
{
	ConsoleViewItem item = {};

	if (wcsncmp(consoleString, ExecuteStrings::EXIT, wcslen(ExecuteStrings::EXIT)) == 0)
	{
		msg.message = WM_QUIT; //TODO: Terrible. Fix.
	}
	else if (wcsncmp(consoleString, ExecuteStrings::GPU, wcslen(ExecuteStrings::GPU)) == 0)
	{
		_snwprintf_s(item.text, sizeof(item.text), L"%ls", dx->adaptersDesc[0].Description);
		viewItems.push_back(item);
	}
	else if (wcsncmp(consoleString, ExecuteStrings::D3D_TIME, wcslen(ExecuteStrings::D3D_TIME)) == 0)
	{
		//TODO: need to figure out a better system to display real time data
		_snwprintf_s(item.text, sizeof(item.text), L"D3D11 Render: %f", dx->renderTime);
		viewItems.push_back(item);
	}
	else if (wcsncmp(consoleString, ExecuteStrings::POP, wcslen(ExecuteStrings::POP)) == 0)
	{
		if (viewItems.size() > 0)
		{
			viewItems.pop_back();
		}
	}
	else if (wcsncmp(consoleString, ExecuteStrings::CLEAR, wcslen(ExecuteStrings::CLEAR)) == 0)
	{
		viewItems.clear();
	}

	//Reset console string and index
	memset(consoleString, 0, _countof(consoleString) * sizeof(wchar_t));
	consoleStringIndex = 0;
}

void Console::DrawViewItems(UIContext* ui)
{
	for (int i = 0; i < viewItems.size(); i++)
	{
		const float yMarginIncrement = 20.f * i;

		ui->d2dRenderTarget->DrawTextA(viewItems[i].text, wcslen(viewItems[i].text), ui->textFormat, 
			{ 0, yMarginIncrement, (float)windowWidth, (float)windowHeight }, ui->brush);

	}
}

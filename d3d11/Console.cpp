#include "Console.h"
#include "Input.h"
#include "UIContext.h"
#include "Win32Util.h"
#include <vector>

wchar_t consoleString[128];
int consoleStringIndex = 0;
bool bConsoleActive = false;

namespace ExecuteStrings
{
	const wchar_t* EXIT = L"EXIT";
}

void Console::ConsoleInput()
{
	if (GetAnyKeyDown())
	{
		if (GetKeyDownState(VK_BACK))
		{
			consoleStringIndex--;
		}
		else if (consoleStringIndex < _countof(consoleString) && currentDownKey != 0)
		{
			consoleString[consoleStringIndex] = currentDownKey; //wchar_t to int64 cast works
			consoleStringIndex++;
		}
	}
}

//Make sure D2D render target calls have been made (Begin/End Draw)
void Console::Tick(UIContext* ui)
{
	if (bConsoleActive)
	{
		Console::ConsoleInput();

		ui->d2dRenderTarget->DrawRectangle({ 0, (float)windowHeight - 20.f, (float)windowWidth, (float)windowHeight }, ui->brush);
		ui->d2dRenderTarget->DrawText(consoleString, consoleStringIndex, ui->textFormat, { 0, (float)windowHeight - 20.f, 1000, 1000 }, ui->brush);
	}

	if (GetKeyDownState(VK_OEM_3)) //~ key, like doom 
	{
		bConsoleActive = !bConsoleActive;
	}
	else if (GetKeyDownState(VK_RETURN) && bConsoleActive)
	{
		Console::ExecuteString();
	}
}

//Execute values need to be uppercase with WndProc
void Console::ExecuteString()
{
	if (wcsncmp(consoleString, ExecuteStrings::EXIT, wcslen(ExecuteStrings::EXIT)) == 0)
	{
		msg.message = WM_QUIT;
	}
}

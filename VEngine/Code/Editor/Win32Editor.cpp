
#include "Win32Editor.h"
#include "imgui_forward_declare.h"
#include "Core/Core.h"
#include "Core/Debug.h"

MSG msg;
HWND window;

void Win32Editor::Init(int argc, char* argv[])
{
	SetViewportDimensions(1000, 600);

	SetupWindow();
	windowHwnd = (void*)window;
}

void Win32Editor::Tick()
{
	SetMousePos();

	HandleMessages();
	Core::HandleWin32MessagePump(msg.message, msg.wParam, msg.lParam);

	ImGui_ImplWin32_WndProcHandler(msg.hwnd, msg.message, msg.wParam, msg.lParam);
}

void Win32Editor::SetMousePos()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient((HWND)windowHwnd, &mousePos);
	SetViewportMousePositions(mousePos.x, mousePos.y);
}

void Win32Editor::Log(const std::wstring logMessage)
{
	OutputDebugStringW(logMessage.c_str());
}

void Win32Editor::Log(const std::string logMessage)
{
	OutputDebugString(logMessage.c_str());
}

void Win32Editor::SetActorProps(Actor* actor)
{
}

void Win32Editor::UpdateWorldList()
{
}

void Win32Editor::AddActorToWorldList(Actor* actor)
{
}

void Win32Editor::RemoveActorFromWorldList()
{
}

void Win32Editor::SetupWindow()
{
	HINSTANCE instance = GetModuleHandle(NULL);

	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = "Window";
	wc.lpfnWndProc = (WNDPROC)Core::WndProc;
	wc.hInstance = instance;
	wc.hCursor = LoadCursor(0, IDC_CROSS);

	RegisterClass(&wc);
	window = CreateWindow("Window", "VEngine2.0", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, GetViewportWidth(), GetViewportHeight(), 0, 0, instance, 0);
	if (!window)
	{
		HR(GetLastError());
	}
	else
	{
		ShowWindow(window, SW_SHOW);
		UpdateWindow(window);

		//Center window
		RECT windowRect; RECT desktopRect;
		GetWindowRect(window, &windowRect);
		GetWindowRect(GetDesktopWindow(), &desktopRect);
		int xPos = (desktopRect.right - (windowRect.right - windowRect.left)) / 2;
		int yPos = (desktopRect.bottom - (windowRect.bottom - windowRect.top)) / 2;

		SetWindowPos(window, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
}

void Win32Editor::HandleMessages()
{
	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Win32Editor::RefreshAssetList()
{
}

void Win32Editor::ClearProperties()
{
}

void Win32Editor::SelectActorInWorldList()
{
}

void Win32Editor::SetPlayButtonText()
{
}

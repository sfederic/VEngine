import Core.Core;
import std.core;

static bool gameWorldPaused = false;

static double ticks = 0.0;
static double deltaTime = 0.0;
static double deltaAccum = 0.0;

static __int64 tickFrequency;
static __int64 frameStartTime;
static __int64 frameEndTime;

static uint32_t frameCount;

namespace Core
{
	std::string initialStartingWorldFromEditor;

	bool mainLoop = true;
	bool gameplayOn = false;
	bool isImGUIEnabled = true;

	double timeSinceStartup;

	uint32_t finalFrameCount;

	float timeScale = 1.0f;
}

float Core::GetDeltaTime()
{
	if (deltaTime < 0.0)
	{
		deltaTime = 0.0;
	}
	else if (deltaTime > 1.0)
	{
		//There's a good talk on the, "Myths and Misconceptions of Frame Pacing", by the Serious Sam guys
		//REF:https://www.youtube.com/watch?v=n0zT8YSSFzw
		deltaTime = 1.0 / 60.0;
	}

	return (float)deltaTime * timeScale;
}

void Core::SetTimerFrequency()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&tickFrequency);
	ticks = 1.0 / (double)tickFrequency;
	deltaAccum = 0.0;
}

void Core::Init()
{
	//For the WIC texture functions from DirectXToolkit and XAudio2 threading
	HR(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED));
	SetTimerFrequency();
}

void Core::Tick()
{
	//Reset current world
	if (Input::GetSystemKeyHeld(Keys::Ctrl))
	{
		if (Input::GetSystemKeyDown(Keys::R))
		{
			FileSystem::ReloadCurrentWorld();
		}
	}

	//Toggle gameplay on/off
	if (Input::GetSystemKeyHeld(Keys::Ctrl))
	{
		if (Input::GetSystemKeyDown(Keys::P))
		{
			SetGameplayState();
		}
	}

	//End game on Esc pressed
	if (Input::GetSystemKeyDown(Keys::Esc))
	{
		if (gameplayOn)
		{
			EndGame();
			gameplayOn = false;
		}
	}
}

void Core::StartTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&frameStartTime);
}

void Core::EndTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&frameEndTime);
	deltaTime = ticks * (double)(frameEndTime - frameStartTime);

	deltaAccum += deltaTime;
	timeSinceStartup += deltaTime;
	frameCount++;

	if (deltaAccum > 1.0)
	{
		finalFrameCount = frameCount;

		frameCount = 0;
		deltaAccum = 0.0;
	}
}

void Core::StartGame()
{
	gameplayOn = true;
	initialStartingWorldFromEditor = World::Get().GetWorldFilename();

	World::Get().StartAllComponents();
	World::Get().WakeAndStartAllActors();

	PhysicsSystem::Start();

	SkeletalMeshComponent::StartAllAnimations();

	GameUtils::LoadGameInstanceData();

	Editor::Get().SetPlayButtonText();
	debugMenu.AddNotification(L"Gameplay started");
}

void Core::EndGame()
{
	World::Get().EndAllActors();

	gameplayOn = false;

	UISystem::Reset();
	SpriteSystem::Reset();
	PhysicsSystem::Reset();
	Timer::Cleanup();
	Profile::Reset();

	AudioSystem::DeleteLoadedAudioAndChannels();
	AudioSystem::StopPersistentTracks();

	SkeletalMeshComponent::StopAllAnimations();

	Camera::SetEditorCameraAsActive();

	//Clear the properties dock or you'll end up with pointer problems
	WorldEditor::DeselectPickedActor();

	Editor::Get().SetPlayButtonText();
	debugMenu.AddNotification(L"Gameplay stopped");
}

void Core::PauseGameWorld()
{
	gameWorldPaused = true;
	debugMenu.AddStaticNotification(L"Game World Paused");
}

void Core::UnPauseGameWorld()
{
	gameWorldPaused = false;
	debugMenu.ClearAllStaticNotifications();
	debugMenu.AddNotification(L"Game World Unpaused");
}

bool Core::IsGameWorldPaused()
{
	return gameWorldPaused;
}

void Core::SetGameplayState()
{
	gameplayOn = !gameplayOn;

	if (gameplayOn)
	{
		StartGame();
	}
	else if (!gameplayOn)
	{
		EndGame();
	}
}

void Core::HandleWin32MessagePump(UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		Input::SetSystemKeyDown((Keys)wparam);
		break;

	case WM_KEYUP:
		Input::SetSystemKeyUp((Keys)wparam);
		break;

	case WM_MBUTTONUP:
		Input::SetMiddleMouseUp();
		break;

	case WM_MBUTTONDOWN:
		Input::SetMiddleMouseDown();
		break;

	case WM_LBUTTONUP:
		Input::SetLeftMouseUp();
		break;

	case WM_LBUTTONDOWN:
		Input::SetLeftMouseDown();
		break;

	case WM_RBUTTONUP:
		Input::SetRightMouseUp();
		break;

	case WM_RBUTTONDOWN:
		Input::SetRightMouseDown();
		break;

	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wparam) < 0)
		{
			Input::SetMouseWheelDown();
		}
		else
		{
			Input::SetMouseWheelUp();
		}
		break;

	case WM_SIZE:
		UINT resizedWidth = LOWORD(lparam);
		UINT resizedHeight = HIWORD(lparam);

		Editor::Get().SetViewportDimensions(resizedWidth, resizedHeight);

		//Minimizing the window for some reason when using Qt returns the wrong resize sizes. With Win32 it works
		//fine, but using the previous width and height here regardless doesn't hurt.
		static UINT previousWidth = 0;
		static UINT previousHeight = 0;

		switch (wparam)
		{
		case SIZE_MINIMIZED: //swapchain resize messes up if minimised (width & height = 0)
			return;
		case SIZE_MAXIMIZED:
			resizedWidth = previousWidth;
			resizedHeight = previousHeight;
			break;
		}

		Renderer::Get().ResizeSwapchain(resizedWidth, resizedHeight);

		previousWidth = resizedWidth;
		previousHeight = resizedHeight;

		break;
	}
}

LRESULT __stdcall Core::WndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	HandleWin32MessagePump(message, wparam, lparam);
	return DefWindowProc(window, message, wparam, lparam);
}

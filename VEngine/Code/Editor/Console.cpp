#include "Console.h"
#include <dwrite.h>
#include <d2d1.h>
#include "Input.h"
#include "UI/UISystem.h"
#include "DebugMenu.h"
#include "Render/Renderer.h"
#include "Asset/AssetSystem.h"
#include "FileSystem.h"

Console console;

Console::Console()
{
	//NOTE: command strings need to be uppercase with WndProc

	//Debug Menu Commands

	//LiSt all console commands
	executeMap.emplace(L"LS", []() { debugMenu.consoleCommandsMenuOpen = !debugMenu.consoleCommandsMenuOpen; });

	//Show snapping menu
	executeMap.emplace(L"SNAP", []() { debugMenu.snapMenuOpen = !debugMenu.snapMenuOpen; });

	//Show profile stats
	executeMap.emplace(L"PROFILE", []() { debugMenu.profileMenuOpen = !debugMenu.profileMenuOpen; });

	//Show FPS and GPU timing info
	executeMap.emplace(L"FPS", []() { debugMenu.fpsMenuOpen = !debugMenu.fpsMenuOpen; });

	//Show actor props
	executeMap.emplace(L"PROPS", []() { debugMenu.propsMenuOpen = !debugMenu.propsMenuOpen; });

	//Shows current undo/redo commands in buffer
	executeMap.emplace(L"COMMANDS", []() { debugMenu.commandsMenuOpen = !debugMenu.commandsMenuOpen; });

	//Show GPU info
	executeMap.emplace(L"GPU", []() { debugMenu.gpuMenuOpen = !debugMenu.gpuMenuOpen; });

	//Shows actor info while hovering over the actor with mouse
	executeMap.emplace(L"ACTOR", []() { debugMenu.actorInspectMenuOpen = !debugMenu.actorInspectMenuOpen; });

	//Show actor system stats
	executeMap.emplace(L"ACTORSYSTEM", []() { debugMenu.actorSystemMenuOpen = !debugMenu.actorSystemMenuOpen; });

	//show component system stats
	executeMap.emplace(L"COMPONENTSYSTEM", []() { debugMenu.componentSystemMenuOpen = !debugMenu.componentSystemMenuOpen; });

	//shows in-world stats (eg. vertex count, actor count)
	executeMap.emplace(L"STATS", []() { debugMenu.worldStatsMenuOpen = !debugMenu.worldStatsMenuOpen; });

	//Menu for manipulating game instance data
	executeMap.emplace(L"GAME", []() { debugMenu.gameInstaceMenuOpen = !debugMenu.gameInstaceMenuOpen; });

	//Show all intuition info player has
	executeMap.emplace(L"INTUITIONS", []() { debugMenu.intuitionsMenuOpen = !debugMenu.intuitionsMenuOpen; });

	//show memory for engine systems
	executeMap.emplace(L"MEM", []() { debugMenu.memoryMenuOpen = !debugMenu.memoryMenuOpen; });

	//Show skeleton heirarchy on actor's meshcomponent
	executeMap.emplace(L"SKEL", []() { debugMenu.skeletonViewMenuOpen = !debugMenu.skeletonViewMenuOpen; });

	//Reload current world
	executeMap.emplace(L"RESET", []() { fileSystem.ReloadCurrentWorld(); });

	//Asset Build Commands
	executeMap.emplace(L"BUILD MESHES", []() { assetSystem.WriteAllMeshDataToMeshAssetFiles(); });

	//Write all game save maps
	executeMap.emplace(L"BUILD MAPS", []() { assetSystem.BuildAllGameplayMapFiles(); });
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

		uiSystem.d2dRenderTarget->DrawRectangle({ 0, height - 50.f, width, height }, uiSystem.debugBrushText);

		uiSystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		uiSystem.d2dRenderTarget->DrawText(consoleString.c_str(), consoleString.size(), uiSystem.textFormat,
			{ 0, height - 50.f, width, height }, uiSystem.debugBrushText);
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

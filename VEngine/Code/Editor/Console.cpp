#include "vpch.h"
#include "Console.h"
#include <dwrite.h>
#include <d2d1.h>
#include "Components/Component.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include "UI/UISystem.h"
#include "UI/Layout.h"
#include "DebugMenu.h"
#include "Render/Renderer.h"
#include "Asset/AssetSystem.h"
#include "Core/FileSystem.h"
#include "Core/World.h"
#include "Core/WorldEditor.h"

std::map<std::wstring, std::pair<std::function<void()>, std::string>> Console::executeMap;

bool Console::bConsoleActive;

std::wstring consoleString;

void Console::Init()
{
	//NOTE: command strings need to be uppercase with WndProc

	//Debug Menu Commands

	executeMap.emplace(L"LS",
		std::make_pair([]() { debugMenu.consoleCommandsMenuOpen = !debugMenu.consoleCommandsMenuOpen; },
		"List all console commands"));

	executeMap.emplace(L"PROFILE",
		std::make_pair([]() { debugMenu.profileMenuOpen = !debugMenu.profileMenuOpen; },
		"Show profile stats"));

	executeMap.emplace(L"FPS",
		std::make_pair([]() { debugMenu.fpsMenuOpen = !debugMenu.fpsMenuOpen; },
		"Show FPS and GPU timing info"));

	executeMap.emplace(L"GPU",
		std::make_pair([]() { debugMenu.gpuMenuOpen = !debugMenu.gpuMenuOpen; },
		"Show GPU info"));

	executeMap.emplace(L"ACTOR",
		std::make_pair([]() { debugMenu.actorInspectMenuOpen = !debugMenu.actorInspectMenuOpen; },
		"Shows actor info while hovering over the actor with mouse"));

	executeMap.emplace(L"ACTORSYSTEM",
		std::make_pair([]() { debugMenu.actorSystemMenuOpen = !debugMenu.actorSystemMenuOpen; },
		"Show actor system stats"));

	executeMap.emplace(L"COMPONENTSYSTEM",
		std::make_pair([]() { debugMenu.componentSystemMenuOpen = !debugMenu.componentSystemMenuOpen; },
		"Show component system stats"));

	executeMap.emplace(L"STATS",
		std::make_pair([]() { debugMenu.worldStatsMenuOpen = !debugMenu.worldStatsMenuOpen; },
		"shows in - world stats (e.g.vertex count, actor count)"));

	executeMap.emplace(L"GAME",
		std::make_pair([]() { debugMenu.gameInstaceMenuOpen = !debugMenu.gameInstaceMenuOpen; },
		"Menu for manipulating game instance data"));

	executeMap.emplace(L"PARTICLE",
		std::make_pair([]() { debugMenu.particleMenuOpen = !debugMenu.particleMenuOpen; },
		"Particle"));

	executeMap.emplace(L"SKEL",
		std::make_pair([]() { debugMenu.skeletonViewMenuOpen = !debugMenu.skeletonViewMenuOpen; },
		"Show skeleton heirarchy on actor's meshcomponent"));

	executeMap.emplace(L"ANIM",
		std::make_pair([]() { debugMenu.skeletalAnimationMenuOpen = !debugMenu.skeletalAnimationMenuOpen; },
			"Show selected skeleton's animations"));

	executeMap.emplace(L"RESET",
		std::make_pair([]() { FileSystem::ReloadCurrentWorld(); },
		"Reload current world"));

	executeMap.emplace(L"CORE",
		std::make_pair([]() { debugMenu.coreMenuOpen = !debugMenu.coreMenuOpen; },
		"Show core engine variables"));

	executeMap.emplace(L"BAKE",
		std::make_pair([]() { Renderer::RenderLightProbeViews(); },
		"Work through light probes in map and get their RBG values from a cubemap rendering"));

	executeMap.emplace(L"BIN",
		std::make_pair([]() { FileSystem::WriteAllSystemsToBinary(); },
		"Save current world to binary format"));

	executeMap.emplace(L"LOADBIN",
		std::make_pair([]() { FileSystem::ReadAllSystemsFromBinary(); },
		"Load current world from existing binary file"));

	executeMap.emplace(L"BUILD MESH",
		std::make_pair([]() { AssetSystem::BuildAllVMeshDataFromFBXImport(); },
		"Build meshes as their engine specific file format."));

	executeMap.emplace(L"BUILD ANIM",
		std::make_pair([]() { AssetSystem::BuildAllAnimationFilesFromFBXImport(); },
			"Build meshes as their engine specific file format."));

	executeMap.emplace(L"BUILD MAPS",
		std::make_pair([]() { AssetSystem::BuildAllGameplayMapFiles(); },
		"Write all game save maps."));

	executeMap.emplace(L"DEFAULT",
		std::make_pair([]() { World::CreateDefaultMapActors(); },
		"Load in default actors for most worlds (Player, Grid, DirectionalLight, etc.)"));

	executeMap.emplace(L"TEXTURE",
		std::make_pair([]() { WorldEditor::texturePlacement = !WorldEditor::texturePlacement; },
		"Enable texture placement mode in editor"));

	executeMap.emplace(L"MATERIAL",
		std::make_pair([]() { WorldEditor::materialPlacement = !WorldEditor::materialPlacement; },
		"Enable material placement mode in editor"));

	executeMap.emplace(L"MS",
		std::make_pair([]() { Renderer::MeshIconImageCapture(); },
		"Take an editor icon image of the currently selected mesh (MS stands for Mesh Snap, like Pokemon 'Snap')"));

	executeMap.emplace(L"WORLD",
		std::make_pair([]() { debugMenu.worldMenuOpen = !debugMenu.worldMenuOpen; },
			"Menu to open worlds at runtime during gameplay."));

	executeMap.emplace(L"PAINT",
		std::make_pair([]() { 
			debugMenu.vertexPaintMenuOpen = !debugMenu.vertexPaintMenuOpen;
			WorldEditor::vertexPaintActive = !WorldEditor::vertexPaintActive;
		}, "Activate vertex paint mode."));

	executeMap.emplace(L"RESET UID",
		std::make_pair([]() { 
				ResetUIDCache();
				auto components = World::GetAllComponentsInWorld();
				for (auto component : components)
				{
					component->SetUID(GenerateUID());
				}
				Log("Reset UIDs of %d components.", components.size());
			},
			"Resets all component UIDs in world."));
}

void Console::ConsoleInput()
{
	if (Input::GetAnyKeyUp())
	{
		if (Input::GetKeyUp(Keys::BackSpace) && !consoleString.empty())
		{
			consoleString.pop_back();
		}
		else
		{
			std::set<Keys> upKeys = Input::GetAllUpKeys();
			for (Keys key : upKeys)
			{
				consoleString.push_back((int)key);
			}
		}
	}
}

//Make sure D2D render target calls have been made (Begin/End Draw)
void Console::Tick()
{
	if (bConsoleActive)
	{
		float width = (float)Renderer::GetViewportWidth();
		float height = (float)Renderer::GetViewportHeight();

		auto rectLayout = Layout({ 0, height - 50.f, width, height });
		UISystem::DrawRect(rectLayout, Colours::Green, 2.f);

		auto textLayout = Layout({ 0, height - 50.f, width, height });
		UISystem::TextDraw(consoleString, textLayout, DWRITE_TEXT_ALIGNMENT_JUSTIFIED, Colours::Green);
	}
}

void Console::InputTick()
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
			ExecuteString();
			bConsoleActive = false;
			return;
		}

		ConsoleInput();
	}
}

void Console::ExecuteString()
{
	auto executeIt = executeMap.find(consoleString);
	if (executeIt != executeMap.end())
	{
		executeIt->second.first();
	}
	else
	{
		debugMenu.AddNotification(L"No command found");
	}

	consoleString.clear();
}

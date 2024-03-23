#include "vpch.h"
#include "Console.h"
#include <dwrite.h>
#include <filesystem>
#include "Actors/DiffuseProbeMap.h"
#include "Components/MeshComponent.h"
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

static void RunWorldLoadTest()
{
	for (auto& entry : std::filesystem::directory_iterator("WorldMaps"))
	{
		std::string filename = entry.path().filename().string();
		if (entry.path().filename().extension() == ".vmap") //Skip over binary folder and files
		{
			Log("Starting world load for [%s].", filename.c_str());
			FileSystem::LoadWorld(filename);
			Log("World load [%s] success.", filename.c_str());
		}
	}
}

void Console::Init()
{
	//NOTE: command strings need to be uppercase with WndProc

	//Debug Menu Commands

	executeMap.emplace(L"CLEAR",
		std::make_pair([]() {
			World::DestroyAllActorsAndComponentsInWorld();
			},
			"Deletes all actors and components in world."));

	executeMap.emplace(L"WORLDLOADTEST",
		std::make_pair([]() { RunWorldLoadTest(); },
			"Load every world in the world maps folder in sequence."));

	executeMap.emplace(L"LS",
		std::make_pair([]() { debugMenu.consoleCommandsMenuOpen = !debugMenu.consoleCommandsMenuOpen; },
			"List all console commands"));

	executeMap.emplace(L"PROF",
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

	executeMap.emplace(L"TEX",
		std::make_pair([]() { WorldEditor::texturePlacement = !WorldEditor::texturePlacement; },
			"Enable texture placement mode in editor"));

	executeMap.emplace(L"ENT",
		std::make_pair([]() {
			WorldEditor::entranceTriggerWorldLoadMode = !WorldEditor::entranceTriggerWorldLoadMode;
			WorldEditor::entranceTriggerWorldLoadMode ?
				debugMenu.AddStaticNotification(L"EntranceTrigger Load Mode Active") :
				debugMenu.ClearAllStaticNotifications();
			},
			"Enable/Disable loading maps based on clicking an EntranceTrigger in-world."));

	executeMap.emplace(L"MESH",
		std::make_pair([]() {
			WorldEditor::meshPlacement = !WorldEditor::meshPlacement;
			WorldEditor::meshPlacement ? debugMenu.AddStaticNotification(L"MeshReplace Mode Active") :
				debugMenu.ClearAllStaticNotifications();
			},
			"Toggle mesh component placement mode in editor."));

	executeMap.emplace(L"PARENT",
		std::make_pair([]() {
			WorldEditor::parentSetActive = !WorldEditor::parentSetActive;
			WorldEditor::parentSetActive ? debugMenu.AddStaticNotification(L"ParentSet Mode Active") :
				debugMenu.ClearAllStaticNotifications();
			},
			"Enable setting parent to currently selected actor in editor."));

	executeMap.emplace(L"MATERIAL",
		std::make_pair([]() { WorldEditor::materialPlacement = !WorldEditor::materialPlacement; },
			"Enable material placement mode in editor"));

	executeMap.emplace(L"UV",
		std::make_pair([]() { debugMenu.uvMenuOpen = !debugMenu.uvMenuOpen; },
			"Menu to set UV values for selected actor's materials."));

	executeMap.emplace(L"MESHICON",
		std::make_pair([]() { Renderer::MeshIconImageCapture(); },
			"Take an editor icon image of the currently selected mesh."));

	executeMap.emplace(L"MAPICON",
		std::make_pair([]() { Renderer::MapIconImageCapture(); },
			"Take an editor icon image of the currently loaded level."));

	executeMap.emplace(L"WORLD",
		std::make_pair([]() { debugMenu.worldMenuOpen = !debugMenu.worldMenuOpen; },
			"Menu to open worlds at runtime during gameplay."));

	executeMap.emplace(L"REP",
		std::make_pair([]() {
			WorldEditor::actorReplaceModeActive = !WorldEditor::actorReplaceModeActive;
			WorldEditor::actorReplaceModeActive ? debugMenu.AddStaticNotification(L"Actor Replace Mode Active") :
				debugMenu.ClearAllStaticNotifications();
			},
			"Replace clicked actor in world with the currently selected actor system."));

	executeMap.emplace(L"MOV",
		std::make_pair([]() {
			WorldEditor::moveActorViaKeyboardInput = !WorldEditor::moveActorViaKeyboardInput;
			WorldEditor::actorReplaceModeActive ? debugMenu.AddStaticNotification(L"ActorKeyboardMove Mode Active") :
				debugMenu.ClearAllStaticNotifications();
			},
			"Set World Editor to move selected actor via keyboard input."));

	executeMap.emplace(L"PAINT",
		std::make_pair([]() {
			debugMenu.vertexPaintMenuOpen = !debugMenu.vertexPaintMenuOpen;
			WorldEditor::vertexPaintActive = !WorldEditor::vertexPaintActive;
			}, "Activate vertex paint mode."));

	executeMap.emplace(L"UVP",
		std::make_pair([]() {
			WorldEditor::uvPaintActive = !WorldEditor::uvPaintActive;
			debugMenu.uvPaintMenuOpen = !debugMenu.uvPaintMenuOpen;
			debugMenu.CreateUVPaintWidget();
			debugMenu.SetUVPaintMenuTextureFilename();
			}, "Activate UV paint mode."));

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

	executeMap.emplace(L"DELPROBE",
		std::make_pair([]() {
			auto probeMap = DiffuseProbeMap::system.GetFirstActor();
			if (probeMap)
			{
				probeMap->DeleteLightProbeDataFile();
			}
			else
			{
				Log("No DiffuseLightMap in level.");
			}
			}, "Deletes this world's light probe data file."));

	executeMap.emplace(L"TEXOFF",
		std::make_pair([]() {
			for (auto& mesh : MeshComponent::system.GetComponents())
			{
				mesh->SetUseTexture(false);
			}
			}, "Sets all materials in world to NOT use texture via material shader data."));

	executeMap.emplace(L"TEXON",
		std::make_pair([]() {
			for (auto& mesh : MeshComponent::system.GetComponents())
			{
				mesh->SetUseTexture(true);
			}
			}, "Sets all materials in world to USE texture via material shader data."));
}

void Console::ConsoleInput()
{
	if (Input::GetKeyDown(Keys::BackSpace) && !consoleString.empty())
	{
		consoleString.pop_back();
	}
	else
	{
		auto keys = Input::GetAllDownKeys();
		for (auto key : keys)
		{
			consoleString.push_back((int)key);
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
	if (Input::GetKeyDown(Keys::Tilde)) //~ key, like doom and unreal
	{
		bConsoleActive = !bConsoleActive;
		consoleString.clear();
		return;
	}

	if (bConsoleActive)
	{
		if (Input::GetKeyDown(Keys::Enter))
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

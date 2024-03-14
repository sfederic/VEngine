#include "vpch.h"
#include "WorldFunctions.h"
#include "Core/Log.h"
#include "Gameplay/GameInstance.h"
#include "Components/MeshComponent.h"

static std::map<std::string, std::function<void()>> worldStartFunctionMap;

//Map that sets up the world filename (e.g. town.vmap) to the world's name to be displayed in game (e.g. "Town")
static std::unordered_map<std::string, std::string> worldFilenamesToInGameNames;

void TownStart()
{
	const auto MulVertexColours = [](XMFLOAT4 colour)
		{
			for (auto& mesh : MeshComponent::system.GetComponents())
			{
				const auto fillColour = XMLoadFloat4(&colour);
				for (auto& vertex : mesh->GetAllVertices())
				{
					auto colour = XMLoadFloat4(&vertex.colour);
					colour *= fillColour;
					XMStoreFloat4(&vertex.colour, colour);
				}

				mesh->CreateNewVertexBuffer();
			}
		};

	std::string* timeOfDay = GameInstance::GetGlobalProp<std::string>("TownTimeOfDay");
	if (timeOfDay == nullptr)
	{
		return;
	}
	if (*timeOfDay == "Morning")
	{
		MulVertexColours(XMFLOAT4(0.3f, 0.3f, 0.88f, 1.f));
	}
	else if (*timeOfDay == "Day")
	{
		MulVertexColours(XMFLOAT4(0.77f, 0.77f, 0.77f, 1.f));
	}
	else if (*timeOfDay == "Sunset")
	{
		MulVertexColours(XMFLOAT4(0.77f, 0.1f, 0.1f, 1.f));
	}
	else if (*timeOfDay == "Night")
	{
		MulVertexColours(XMFLOAT4(0.1f, 0.1f, 0.69f, 1.f));
	}
}

void WorldFunctions::SetupWorldNames()
{
	//CHURCH
	worldFilenamesToInGameNames.emplace("church_balcony_room.vmap", "Dining Room");
	worldFilenamesToInGameNames.emplace("church_entrance.vmap", "Entrance Hall");
	worldFilenamesToInGameNames.emplace("church_foyer.vmap", "Foyer");
	worldFilenamesToInGameNames.emplace("church_library.vmap", "Library");
	worldFilenamesToInGameNames.emplace("church_planetarium.vmap", "Planetarium");
	worldFilenamesToInGameNames.emplace("church_projection_crystal_prayer_room.vmap", "Prayer Room");
	worldFilenamesToInGameNames.emplace("church_ruins.vmap", "Ruins");
	worldFilenamesToInGameNames.emplace("church_sanctuary.vmap", "Sanctuary");
	worldFilenamesToInGameNames.emplace("church_sundial_garden.vmap", "Garden");
	worldFilenamesToInGameNames.emplace("church_upstairs.vmap", "Main Hall");
}

std::string WorldFunctions::GetWorldDisplayName(std::string mapFilename)
{
	auto mapIt = worldFilenamesToInGameNames.find(mapFilename);
	if (mapIt == worldFilenamesToInGameNames.end())
	{
		return "NO NAME FOUND";
	}
	return mapIt->second;
}

void WorldFunctions::SetupWorldStartFunctions()
{
	worldStartFunctionMap.emplace("town.vmap", &TownStart);
}

void WorldFunctions::CallWorldStartFunction(const std::string worldName)
{
	auto funcIt = worldStartFunctionMap.find(worldName);
	if (funcIt != worldStartFunctionMap.end())
	{
		funcIt->second();
	}
	else
	{
		Log("No Start function for [%s] world.", worldName.c_str());
	}
}

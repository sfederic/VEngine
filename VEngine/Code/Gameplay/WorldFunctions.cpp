#include "vpch.h"
#include "WorldFunctions.h"
#include "Core/Log.h"
#include "Gameplay/GameInstance.h"
#include "Components/MeshComponent.h"

std::map<std::string, std::function<void()>> worldStartFunctionMap;

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

	const auto timeOfDay = GameInstance::GetGlobalProp<std::string>("TownTimeOfDay");
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

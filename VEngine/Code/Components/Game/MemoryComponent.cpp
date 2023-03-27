#include "vpch.h"
#include "MemoryComponent.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "Core/Log.h"
#include "Core/VString.h"
#include "Core/World.h"
#include "Gameplay/Memory.h"
#include "Gameplay/ConditionSystem.h"
#include "Core/Timer.h"
#include "UI/Game/MemoryGainedWidget.h"
#include "UI/UISystem.h"
#include "Audio/AudioSystem.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Player.h"

MemoryComponent::MemoryComponent()
{
}

Properties MemoryComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = "MemoryComponent";
	props.Add("Memory Name", &memoryName);
	props.Add("Add On Interact", &addOnInteract);
	return props;
}

bool MemoryComponent::CreateMemory(std::string actorAquiredFromName)
{
	if (memoryName.empty())
	{
		Log("%s Memory name not set.", actorAquiredFromName.c_str());
		return false;
	}

	auto memoryIt = GameInstance::playerMemories.find(VString::wstos(memoryName));
	if (memoryIt != GameInstance::playerMemories.end())
	{
		Log("%s Memory already known.", memoryName.c_str());
		return false;
	}

	auto memory = Memory::FindMemory(VString::wstos(memoryName));

	memory->actorAquiredFrom = actorAquiredFromName;
	memory->worldAquiredFrom = World::worldFilename;

	if (!memory->CheckCondition())
	{
		return false;
	}
	if (!memory->RunCondition())
	{
		return false;
	}

	auto player = Player::system.GetFirstActor();

	//Create Memory for player
	GameInstance::playerMemories.emplace(memory->name, memory);
	Log("%s memory created.", memory->name.c_str());

	UISystem::memoryGainedWidget->memoryToDisplay = GameInstance::playerMemories[memory->name];
	UISystem::memoryGainedWidget->AddToViewport();

	//Mute all channels because Memory Gained sound fucking with the musical key
	AudioSystem::FadeOutAllAudio();
	GameUtils::PlayAudioOneShot("intuition_gained.wav");
	Timer::SetTimer(5.0f, AudioSystem::FadeInAllAudio);

	return true; //memory created
}

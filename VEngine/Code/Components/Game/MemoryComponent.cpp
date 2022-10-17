#include "vpch.h"
#include "MemoryComponent.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "Log.h"
#include "VString.h"
#include "World.h"
#include "Gameplay/Memory.h"
#include "Gameplay/ConditionSystem.h"
#include "Timer.h"
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
	props.Add("Memory Desc", &memoryDescription);
	props.Add("Condition", &condition);
	props.Add("Condition Value", &conditionArg);
	props.Add("Add On Interact", &addOnInteract);
	props.Add("Image File", &imageFile);
	props.Add("Atk Increase", &attackIncrease);
	props.Add("Life Increase", &lifeIncrease);
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

	//INIT MEMORY
	auto memory = new Memory(VString::wstos(memoryName));

	//For spawning meshes attribute to a memory when remembering it
	Actor* owner = nullptr;
	if (!actorAquiredFromName.empty())
	{
		owner = World::GetActorByName(actorAquiredFromName);

		memory->spawnActorSystem = owner->GetActorSystem();

		auto meshes = owner->GetComponentsOfType<MeshComponent>();
		memory->meshName = meshes[0]->meshComponentData.filename;
	}

	memory->description = VString::wstos(memoryDescription);

	memory->actorAquiredFrom = actorAquiredFromName;
	memory->worldAquiredFrom = World::worldFilename;

	memory->imageFile = this->imageFile;

	//Check if memory condition passes
	if (!condition.empty())
	{
		memory->conditionFunc = conditionSystem.FindCondition(condition);
		memory->conditionFuncName = condition;

		if (!memory->conditionFunc(conditionArg))
		{
			return false; //memory not created
		}
	}

	//Increase player stats if part of memory
	memory->attackIncrease = attackIncrease;
	memory->lifeIncrease = lifeIncrease;

	auto player = Player::system.GetFirstActor();
	player->attackPoints += memory->attackIncrease;
	player->healthPoints += memory->lifeIncrease;

	//Create Memory
	GameInstance::playerMemories.emplace(memory->name, memory);
	Log("%s memory created.", memory->name.c_str());

	uiSystem.memoryGainedWidget->memoryToDisplay = GameInstance::playerMemories[memory->name];
	uiSystem.memoryGainedWidget->AddToViewport();

	//Mute all channels because Memory Gained sound fucking with the musical key
	audioSystem.FadeOutAllAudio();
	GameUtils::PlayAudioOneShot("intuition_gained.wav");
	Timer::SetTimer(5.0f, std::bind(&AudioSystem::FadeInAllAudio, &audioSystem));

	return true; //memory created
}

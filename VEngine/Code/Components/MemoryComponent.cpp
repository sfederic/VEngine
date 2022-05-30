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
#include "UI/MemoryGainedWidget.h"
#include "UI/UISystem.h"
#include "Audio/AudioSystem.h"
#include "Components/MeshComponent.h"

MemoryComponent::MemoryComponent()
{
}

Properties MemoryComponent::GetProps()
{
	Properties props(name.c_str());
	props.Add("Memory Name", &memoryName);
	props.Add("Memory Desc", &memoryDescription);
	props.Add("Condition", &condition);
	props.Add("Condition Value", &conditionArg);
	props.Add("Add On Interact", &addOnInteract);
	props.Add("Image File", &imageFile);
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

	auto owner = world.GetActorByName(actorAquiredFromName);

	auto memory = new Memory();

	memory->name = VString::wstos(memoryName);
	memory->description = VString::wstos(memoryDescription);

	memory->actorAquiredFrom = actorAquiredFromName;
	memory->worldAquiredFrom = world.worldFilename;

	memory->hourAquired = GameInstance::currentHour;
	memory->minuteAquired = GameInstance::currentMinute;

	memory->spawnActorSystem = owner->actorSystem;

	auto meshes = owner->GetComponentsOfType<MeshComponent>();
	memory->meshName = meshes[0]->meshComponentData.filename;

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

	//Create intuiton
	GameInstance::playerMemories.emplace(memory->name, memory);
	Log("%s memory created.", memory->name.c_str());

	uiSystem.memoryGainedWidget->memoryToDisplay = GameInstance::playerMemories[memory->name];
	uiSystem.memoryWidgetInViewport = true;
	uiSystem.memoryGainedWidget->AddToViewport();

	//Mute all channels because Memory Gained sound fucking with the musical key
	audioSystem.FadeOutAllAudio();
	GameUtils::PlayAudioOneShot("intuition_gained.wav");
	Timer::SetTimer(5.0f, std::bind(&AudioSystem::FadeInAllAudio, &audioSystem));

	return true; //memory created
}

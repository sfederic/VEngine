#include "vpch.h"
#include "ConditionSystem.h"
#include <cassert>
#include "GameInstance.h"
#include "GameUtils.h"
#include "Memory.h"
#include "UI/UISystem.h"
#include "UI/Game/MemoryRecalledWidget.h"
#include "Gameplay/BattleSystem.h"
#include "Core/World.h"
#include "Core/Log.h"
#include "UI/Game/MemoryGainedWidget.h"
#include "Audio/AudioSystem.h"
#include "Actors/Game/EntranceTrigger.h"
#include "Actors/Game/Player.h"
#include "Components/MeshComponent.h"

ConditionSystem conditionSystem;

//CONDITION FUNCTIONS

bool AttackUp(std::string arg)
{
	auto player = Player::system.GetFirstActor();
	try
	{
		player->attackPoints += std::stoi(arg);
	}
	catch (std::exception ex)
	{
		throw;
	}

	return true;
}

bool UnlockEntrance(std::string arg)
{
	Actor* actor = World::GetActorByName(arg);
	auto entranceTrigger = dynamic_cast<EntranceTrigger*>(actor);
	assert(entranceTrigger);
	entranceTrigger->UnlockEntrance();
	return true;
}

bool PlaySong(std::string arg)
{
	AudioSystem::MuteAllAudio();
	AudioSystem::PlayAudio(arg, true);
	return true;
}

bool MemoryCheck(std::string arg)
{
	auto memoryIt = GameInstance::playerMemories.find(arg);
	if (memoryIt != GameInstance::playerMemories.end())
	{
		//UISystem::memoryRecalledWidget->recalledMemory = memoryIt->second;
		//UISystem::memoryRecalledWidget->AddToViewport();

		GameUtils::PlayAudioOneShot("intuition_check_success.wav");

		return true;
	}

	Log("MemoryCheck [%s] not found.", arg.c_str());
	return false;
}

bool StartBattle(std::string arg)
{
	battleSystem.StartBattle();
	return true;
}

bool GainMemory(std::string arg)
{
	size_t firstOf = arg.find("|");
	size_t lastOf = arg.find_last_of("|");

	std::string memoryName = arg.substr(0, firstOf);
	std::string memoryDesc = arg.substr(memoryName.size() + 1, lastOf - memoryName.size() - 1);
	std::string memoryImage = arg.substr(lastOf + 1);

	auto memory = new Memory(memoryName);
	memory->description = memoryDesc;

	memory->actorAquiredFrom = "Aquired from dialogue.";
	memory->worldAquiredFrom = World::worldFilename;

	memory->imageFile = memoryImage;

	GameInstance::playerMemories.emplace(memory->name, memory);
	Log("%s Memory created.", memory->name.c_str());

	//UISystem::memoryGainedWidget->memoryToDisplay = GameInstance::playerMemories[memory->name];
	//UISystem::memoryGainedWidget->AddToViewport();

	GameUtils::PlayAudioOneShot("purchase.wav");

	return true;
}

//END CONDITION FUNCTIONS

ConditionSystem::ConditionSystem()
{
	ADD_CONDITION(MemoryCheck);
	ADD_CONDITION(StartBattle);
	ADD_CONDITION(GainMemory);
	ADD_CONDITION(PlaySong);
	ADD_CONDITION(UnlockEntrance);
}

void ConditionSystem::AddCondition(std::string functionName, ConditionFunction conditionFunction)
{
	conditions.insert(std::make_pair(functionName, conditionFunction));
}

ConditionFunction ConditionSystem::FindCondition(std::string conditionName)
{
	auto conditionIt = conditions.find(conditionName);
	assert(conditionIt != conditions.end());
	return conditionIt->second;
}

ConditionFunction ConditionSystem::FindConditionAllowNull(std::string conditionName)
{
	auto conditionIt = conditions.find(conditionName);
	if (conditionIt == conditions.end())
	{
		Log("Condition [%s] not found.", conditionName.c_str());
		return nullptr;
	}
	return conditionIt->second;
}

bool ConditionSystem::EvaluateCondition(std::string conditionName, std::string conditionArg)
{
	auto conditionIt = conditions.find(conditionName);
	assert(conditionIt != conditions.end());
	return conditionIt->second(conditionArg);
}

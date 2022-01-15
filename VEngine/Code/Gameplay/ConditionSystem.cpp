#include "ConditionSystem.h"
#include <cassert>
#include "GameInstance.h"
#include "GameUtils.h"
#include "UI/UISystem.h"
#include "UI/MemoryRecalledWidget.h"
#include "Gameplay/BattleSystem.h"
#include "World.h"
#include "Log.h"
#include "UI/UISystem.h"
#include "UI/MemoryGainedWidget.h"
#include "Audio/AudioSystem.h"

ConditionSystem conditionSystem;

//CONDITION FUNCTIONS

bool PlaySong(std::string arg)
{
	audioSystem.MuteAllAudio();
	audioSystem.PlayAudio(arg, true);
	return true;
}

bool MemoryCheck(std::string arg)
{
	auto memoryIt = GameInstance::playerMemories.find(arg);
	if (memoryIt != GameInstance::playerMemories.end())
	{
		uiSystem.memoryRecalledWidget->recalledMemory = &memoryIt->second;
		uiSystem.memoryWidgetInViewport = true;
		uiSystem.memoryRecalledWidget->AddToViewport();

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

//Define the memory as "Name|Description|Image Filename" in the dialogue dock
bool GainMemory(std::string arg)
{
	std::string memoryName = arg.substr(0, arg.find("|"));
	std::string memoryDesc = arg.substr(arg.find("|") + 1, arg.find_last_of("|") - memoryName.size());
	std::string memoryImage = arg.substr(arg.find_last_of("|") + 1);

	auto memory = Memory();
	memory.name = memoryName;
	memory.description = memoryDesc;

	memory.actorAquiredFrom = "Aquired from dialogue.";
	memory.worldAquiredFrom = world.worldFilename;

	memory.hourAquired = GameInstance::currentHour;
	memory.minuteAquired = GameInstance::currentMinute;

	memory.imageFile = memoryImage;

	GameInstance::playerMemories.emplace(memory.name, memory);
	Log("%s Memory created.", memory.name.c_str());

	uiSystem.memoryGainedWidget->memoryToDisplay = &GameInstance::playerMemories[memory.name];
	uiSystem.memoryWidgetInViewport = true;
	uiSystem.memoryGainedWidget->AddToViewport();

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
	auto& foundCondition = conditions[conditionName];
	return foundCondition;
}

bool ConditionSystem::EvaluateCondition(std::string conditionName, std::string conditionArg)
{
	auto conditionIt = conditions.find(conditionName);
	assert(conditionIt != conditions.end());
	return conditionIt->second(conditionArg);
}

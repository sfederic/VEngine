#include "vpch.h"
#include "ConditionSystem.h"
#include <cassert>
#include "GameInstance.h"
#include "GameUtils.h"
#include "Memory.h"
#include "UI/UISystem.h"
#include "UI/MemoryRecalledWidget.h"
#include "Gameplay/BattleSystem.h"
#include "World.h"
#include "Log.h"
#include "UI/MemoryGainedWidget.h"
#include "Audio/AudioSystem.h"
#include "Quests/QuestSystem.h"
#include "Quests/Quest.h"

ConditionSystem conditionSystem;

//CONDITION FUNCTIONS

static bool PlaySong(std::string arg)
{
	audioSystem.MuteAllAudio();
	audioSystem.PlayAudio(arg, true);
	return true;
}

static bool MemoryCheck(std::string arg)
{
	auto memoryIt = GameInstance::playerMemories.find(arg);
	if (memoryIt != GameInstance::playerMemories.end())
	{
		uiSystem.memoryRecalledWidget->recalledMemory = memoryIt->second;
		uiSystem.memoryWidgetInViewport = true;
		uiSystem.memoryRecalledWidget->AddToViewport();

		GameUtils::PlayAudioOneShot("intuition_check_success.wav");

		return true;
	}

	Log("MemoryCheck [%s] not found.", arg.c_str());
	return false;
}

static bool StartBattle(std::string arg)
{
	battleSystem.StartBattle();
	return true;
}

static bool EndQuest(std::string arg)
{
	Quest* quest = questSystem.FindQuest(arg.c_str());
	if (quest) 
	{
		quest->isActive = false;
	}

	return false;
}

//Define the memory as "Name|Description|Image Filename" in the dialogue dock
static bool GainMemory(std::string arg)
{
	size_t firstOf = arg.find("|");
	size_t lastOf = arg.find_last_of("|");

	std::string memoryName = arg.substr(0, firstOf);
	std::string memoryDesc = arg.substr(memoryName.size() + 1, lastOf - memoryName.size() - 1);
	std::string memoryImage = arg.substr(lastOf + 1);

	auto memory = new Memory();
	memory->name = memoryName;
	memory->description = memoryDesc;

	memory->actorAquiredFrom = "Aquired from dialogue.";
	memory->worldAquiredFrom = world.worldFilename;

	memory->hourAquired = GameInstance::currentHour;
	memory->minuteAquired = GameInstance::currentMinute;

	memory->imageFile = memoryImage;

	GameInstance::playerMemories.emplace(memory->name, memory);
	Log("%s Memory created.", memory->name.c_str());

	uiSystem.memoryGainedWidget->memoryToDisplay = GameInstance::playerMemories[memory->name];
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
	ADD_CONDITION(EndQuest);
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

#include "vpch.h"
#include "ConditionSystem.h"
#include <cassert>
#include "GameInstance.h"
#include "GameUtils.h"
#include "Memory.h"
#include "UI/UISystem.h"
#include "World.h"
#include "Log.h"
#include "Audio/AudioSystem.h"

ConditionSystem conditionSystem;

//CONDITION FUNCTIONS --------------------------------------------------------|

static bool PlaySong(std::string arg)
{
	audioSystem.MuteAllAudio();
	audioSystem.PlayAudio(arg, true);
	return true;
} VFunction<std::string> PlaySongVFunc("PlaySong", &PlaySong, { "Song Name" });

//END CONDITION FUNCTIONS ----------------------------------------------------|

ConditionSystem::ConditionSystem()
{
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

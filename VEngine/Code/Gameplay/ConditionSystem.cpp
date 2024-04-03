#include "vpch.h"
#include "ConditionSystem.h"
#include "Core/World.h"
#include "Core/Log.h"
#include "Audio/AudioSystem.h"
#include "Actors/Game/EntranceTrigger.h"

ConditionSystem conditionSystem;

//CONDITION FUNCTIONS

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
	auto channelID = AudioSystem::LoadAudio(arg, true);
	AudioSystem::PlayAudio(channelID);
	return true;
}

//END CONDITION FUNCTIONS

ConditionSystem::ConditionSystem()
{
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

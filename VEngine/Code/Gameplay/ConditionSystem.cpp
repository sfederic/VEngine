#include "vpch.h"
#include "ConditionSystem.h"
#include <cassert>
#include "GameInstance.h"
#include "GameUtils.h"
#include "Memory.h"
#include "UI/UISystem.h"
#include "Gameplay/BattleSystem.h"
#include "Core/World.h"
#include "Core/Log.h"
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

bool StartBattle(std::string arg)
{
	battleSystem.StartBattle();
	return true;
}

//END CONDITION FUNCTIONS

ConditionSystem::ConditionSystem()
{
	ADD_CONDITION(StartBattle);
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

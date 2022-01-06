#include "ConditionSystem.h"
#include <cassert>
#include "GameInstance.h"
#include "GameUtils.h"
#include "UI/UISystem.h"
#include "UI/IntuitionRecalledWidget.h"
#include "Gameplay/BattleSystem.h"
#include "World.h"
#include "Log.h"
#include "UI/UISystem.h"
#include "UI/IntuitionGainedWidget.h"

ConditionSystem conditionSystem;

//CONDITION FUNCTIONS

bool IntuitionCheck(std::string arg)
{
	auto intuitionIt = GameInstance::playerIntuitions.find(arg);
	if (intuitionIt != GameInstance::playerIntuitions.end())
	{
		uiSystem.intuitionRecalledWidget->recalledIntuition = &intuitionIt->second;
		uiSystem.intuitionWidgetInViewport = true;
		uiSystem.intuitionRecalledWidget->AddToViewport();

		GameUtils::PlayAudioOneShot("intuition_check_success.wav");

		return true;
	}

	return false;
}

bool StartBattle(std::string arg)
{
	battleSystem.StartBattle();
	return true;
}

//Define the intuition as "Name|Description" in the dialogue dock
bool GainIntuition(std::string arg)
{
	std::string intuitionName = arg.substr(0, arg.find("|"));
	std::string intuitionDesc = arg.substr(arg.find("|") + 1);

	auto intuition = Intuition();
	intuition.name = intuitionName;
	intuition.description = intuitionDesc;

	intuition.actorAquiredFrom = "Aquired from dialogue.";
	intuition.worldAquiredFrom = world.worldFilename;

	intuition.hourAquired = GameInstance::currentHour;
	intuition.minuteAquired = GameInstance::currentMinute;

	GameInstance::playerIntuitions.emplace(intuition.name, intuition);
	Log("%s Intuition created.", intuition.name.c_str());

	uiSystem.intuitionGainedWidget->intuitionToDisplay = &GameInstance::playerIntuitions[intuition.name];
	uiSystem.intuitionWidgetInViewport = true;
	uiSystem.intuitionGainedWidget->AddToViewport();

	GameUtils::PlayAudioOneShot("purchase.wav");

	return true;
}

//END CONDITION FUNCTIONS

ConditionSystem::ConditionSystem()
{
	ADD_CONDITION(IntuitionCheck);
	ADD_CONDITION(StartBattle);
	ADD_CONDITION(GainIntuition);
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

#include "ConditionSystem.h"
#include <cassert>
#include "GameInstance.h"
#include "GameUtils.h"
#include "UI/UISystem.h"
#include "UI/IntuitionRecalledWidget.h"
#include "Gameplay/BattleSystem.h"

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

//END CONDITION FUNCTIONS

ConditionSystem::ConditionSystem()
{
	ADD_CONDITION(IntuitionCheck);
	ADD_CONDITION(StartBattle);
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

bool ConditionSystem::EvaluateCondition(std::string conditionName, std::string conditionArg)
{
	auto conditionIt = conditions.find(conditionName);
	assert(conditionIt != conditions.end());
	return conditionIt->second(conditionArg);
}

#include "ConditionSystem.h"
#include <cassert>
#include "GameInstance.h"

ConditionSystem conditionSystem;

//CONDITION FUNCTIONS

bool IntuitionCheck(std::string arg)
{
	auto intuitionIt = GameInstance::playerIntuitions.find(arg);
	return intuitionIt != GameInstance::playerIntuitions.end();
}

//END CONDITION FUNCTIONS

ConditionSystem::ConditionSystem()
{
	ADD_CONDITION(IntuitionCheck);
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

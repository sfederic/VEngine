#include "ConditionSystem.h"
#include <cassert>

ConditionSystem conditionSystem;

bool TestConditionFunction()
{
	return true;
}

ConditionSystem::ConditionSystem()
{
	ADD_CONDITION(TestConditionFunction);
}

void ConditionSystem::AddCondition(std::string functionName, std::function<bool()> conditionFunction)
{
	conditions.insert(std::make_pair(functionName, conditionFunction));
}

bool ConditionSystem::EvaluateCondition(std::string conditionName)
{
	auto conditionIt = conditions.find(conditionName);
	assert(conditionIt != conditions.end());
	return conditionIt->second();
}

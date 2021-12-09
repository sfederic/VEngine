#include "ConditionSystem.h"
#include <cassert>

ConditionSystem conditionSystem;

//Global Condition functions
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

std::function<bool()> ConditionSystem::FindCondition(std::string conditionName)
{
	auto conditionIt = conditions.find(conditionName);
	assert(conditionIt != conditions.end());
	return conditionIt->second;
}

bool ConditionSystem::EvaluateCondition(std::string conditionName)
{
	auto conditionIt = conditions.find(conditionName);
	assert(conditionIt != conditions.end());
	return conditionIt->second();
}

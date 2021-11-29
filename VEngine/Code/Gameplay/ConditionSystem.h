#pragma once
#include <functional>
#include <map>
#include <string>
#include <iostream>

//Evaluates and contains all in-game event conditions.
//Condition functions need to be created as free functions in Conditions.cpp and then added to the system.
//through the ADD_CONDITION macro.
struct ConditionSystem
{
	std::map<std::string, std::function<bool()>> conditions;

	ConditionSystem();

	//Don't call this function directly, just used the ADD_CONDITION macro below to grab the function name.
	void AddCondition(std::string functionName, std::function<bool()> conditionFunction);
	std::function<bool()> FindCondition(std::string conditionName);
	bool EvaluateCondition(std::string conditionName);
};

extern ConditionSystem conditionSystem;

#define ADD_CONDITION(func) conditionSystem.AddCondition(#func, func)

#pragma once
#include <functional>
#include <map>
#include <string>
#include <iostream>

//CONDITION FUNCTIONS
//Condition functions need to be created as free functions in Conditions.cpp and then added to the system.
//through the ADD_CONDITION macro in the ConditionSystem constructor.
//When a condition returns true, that means the goto line in the dialogueline will be the next line.
//If the condition just needs to be used as an event (play sound, add intuiton), return false.

//Evaluates and contains all in-game event conditions.
struct ConditionSystem
{
	std::map<std::string, std::function<bool(std::string)>> conditions;

	ConditionSystem();

	//Don't call this function directly, just used the ADD_CONDITION macro below to grab the function name.
	void AddCondition(std::string functionName, std::function<bool(std::string)> conditionFunction);
	std::function<bool(std::string)> FindCondition(std::string conditionName);
	bool EvaluateCondition(std::string conditionName, std::string conditionArg);
};

extern ConditionSystem conditionSystem;

#define ADD_CONDITION(func) conditionSystem.AddCondition(#func, func)

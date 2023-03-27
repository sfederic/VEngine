#pragma once

#include <functional>
#include <map>
#include <string>
#include "Core/VFunction.h"

//CONDITION FUNCTIONS
//Condition functions need to be created as free functions in Conditions.cpp and then added to the system.
//through the ADD_CONDITION macro in the ConditionSystem constructor.

//When a condition returns true, that means the goto line in the dialogueline will be the next line.
//For example a dialogue tree with a memory check, on the line of the memory check, set the 'goto' to the 
//desired line.

//If the condition just needs to be used as an event (play sound, add intuiton), return false.

typedef std::function<bool(std::string)> ConditionFunction;

//@Todo: Conditionsystem and function system are clashing. Get rid of condition system.

//Evaluates and contains all in-game event conditions.
class ConditionSystem
{
private:
	std::map<std::string, ConditionFunction> conditions;

public:
	ConditionSystem();

	//Don't call this function directly, just used the ADD_CONDITION macro below to grab the function name.
	void AddCondition(std::string functionName, ConditionFunction conditionFunction);
	ConditionFunction FindCondition(std::string conditionName);
	ConditionFunction FindConditionAllowNull(std::string conditionName);
	bool EvaluateCondition(std::string conditionName, std::string conditionArg);
	auto& GetConditions() { return conditions; }
};

extern ConditionSystem conditionSystem;

#define ADD_CONDITION(func) conditionSystem.AddCondition(#func, func)

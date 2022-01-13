#pragma once
#include "ConditionSystem.h"

//Base for all player memories in-game.
struct Memory
{
	std::string name;
	std::string description;
	
	//Debug menu information
	std::string conditionFuncName;
	std::string actorAquiredFrom;
	std::string worldAquiredFrom;

	//Time information
	int hourAquired = 0;
	int minuteAquired = 0;

	//Function to call in order to add Memory to Player.
	ConditionFunction conditionFunc;
};

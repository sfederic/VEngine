#pragma once
#include <string>
#include <functional>

//Base for all player intuitions in-game.
struct Intuition
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

	//Function to call in order to add Intuition to Player.
	std::function<bool()> conditionFunc;
};

#pragma once
#include <string>
#include <functional>

//Base for all plyaer intuitions in-game.
struct Intuition
{
	std::string name;
	std::string description;

	//Function to call in order to add Intuition to Player.
	std::function<bool()> conditionFunc;
};

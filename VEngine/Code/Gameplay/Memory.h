#pragma once
#include "ConditionSystem.h"

struct IActorSystem;

//Base for all player memories in-game.
struct Memory
{
	std::string name;
	std::string description;
	
	//Debug menu information
	std::string conditionFuncName;
	std::string actorAquiredFrom;
	std::string worldAquiredFrom;

	//Info for spawning actors from memory
	std::string meshName;
	IActorSystem* spawnActorSystem = nullptr;

	//Time information
	int hourAquired = 0;
	int minuteAquired = 0;

	//Function to call in order to add Memory to Player.
	ConditionFunction conditionFunc;
};

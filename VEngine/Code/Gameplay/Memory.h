#pragma once
#include "ConditionSystem.h"

class IActorSystem;

//Base for all player memories in-game.
struct Memory
{
	virtual ~Memory() {} //Needs to be here for dynamic_cast's, to recognise as polymorphic.

	std::string name;
	std::string description;
	
	//Debug menu information
	std::string conditionFuncName;
	std::string actorAquiredFrom;
	std::string worldAquiredFrom;

	//Info for spawning actors from memory
	std::string meshName;
	IActorSystem* spawnActorSystem = nullptr;

	//Image to display related to memory
	std::string imageFile;

	//Time information
	int hourAquired = 0;
	int minuteAquired = 0;

	//Function to call in order to add Memory to Player.
	ConditionFunction conditionFunc;
};

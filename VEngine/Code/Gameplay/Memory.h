#pragma once

#include <map>
#include "ConditionSystem.h"
#include "Gameplay/MemorySystem.h"

class IActorSystem;

//Base for all player memories in-game.
struct Memory
{
	Memory(std::string name_) : name(name_)
	{
		MemorySystem::Get().CreateMemory(name, this);
	}

	Memory(std::string name_,
		std::string description_,
		std::string conditionFuncName_,
		std::string conditionArg_,
		std::string imageFile_) :
			name(name_),
			description(description_),
			conditionFuncName(conditionFuncName_),
			conditionArg(conditionArg_),
			imageFile(imageFile_)
	{
		MemorySystem::Get().CreateMemory(name, this);
	}

	virtual ~Memory() {} //Needs to be here for dynamic_cast's, to recognise as polymorphic.

	//Code-based condition to check against memory.
	virtual bool RunCondition() { return true; }

	//Editor-based simple condition to check against memory.
	bool CheckCondition();

	std::string name;
	std::string description;
	
	std::string conditionFuncName;
	std::string conditionArg;

	//Image to display related to memory
	std::string imageFile;

	//Debug menu information
	std::string actorAcquiredFrom;
	std::string worldAcquiredFrom;

	//Function to call in order to add Memory to Player.
	ConditionFunction conditionFunc = nullptr;
};

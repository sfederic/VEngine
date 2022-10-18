#pragma once

#include <map>
#include "ConditionSystem.h"

class IActorSystem;

//Base for all player memories in-game.
struct Memory
{
	inline static std::map<std::string, Memory*>* memories = nullptr;

	static Memory* FindMemory(std::string memoryName) 
	{
		auto memoryIt = memories->find(memoryName);
		assert(memoryIt != memories->end());
		return memoryIt->second;
	}

	Memory(std::string name_) : name(name_)
	{
		if (memories == nullptr)
		{
			memories = new std::map<std::string, Memory*>();
		}
		memories->emplace(name, this);
	}

	Memory(std::string name_,
		std::string description_,
		std::string conditionFuncName_,
		std::string conditionArg_,
		std::string imageFile_,
		int attackIncrease_,
		int lifeIncrease_) :
			name(name_),
			description(description_),
			conditionFuncName(conditionFuncName_),
			conditionArg(conditionArg_),
			imageFile(imageFile_),
			attackIncrease(attackIncrease_),
			lifeIncrease(lifeIncrease_) 
	{
		if (memories == nullptr)
		{
			memories = new std::map<std::string, Memory*>();
		}
		memories->emplace(name, this);
	}

	virtual ~Memory() {} //Needs to be here for dynamic_cast's, to recognise as polymorphic.

	bool CheckCondition();

	std::string name;
	std::string description;
	
	std::string conditionFuncName;
	std::string conditionArg;

	//Debug menu information
	std::string actorAquiredFrom;
	std::string worldAquiredFrom;

	//Info for spawning actors from memory
	std::string meshName;
	IActorSystem* spawnActorSystem = nullptr;

	//Image to display related to memory
	std::string imageFile;

	//Vars to either increase or decrease player stats
	int attackIncrease = 0;
	int lifeIncrease = 0;

	//Function to call in order to add Memory to Player.
	ConditionFunction conditionFunc = nullptr;
};

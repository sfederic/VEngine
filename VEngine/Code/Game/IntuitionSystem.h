#pragma once
#include "System.h"
#include <map>
#include "Intuition.h"

//Manages all Intuitions in-game.
struct IntuitionSystem : System
{
	IntuitionSystem();

	std::map<std::string, Intuition*> intuitions;

	template <typename T>
	T* AddIntuition(std::string name)
	{
		auto intuition = new T();
		intuition->name = name;
		intuitions.insert(name, intuition);

		return intuition;
	}
};

extern IntuitionSystem intuitionSystem;

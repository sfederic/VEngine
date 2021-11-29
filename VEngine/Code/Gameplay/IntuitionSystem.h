#pragma once
#include "System.h"
#include <map>
#include "Intuition.h"

//Manages all Intuitions in-game.
struct IntuitionSystem : System
{
	IntuitionSystem();

	std::map<std::string, Intuition*> intuitions;

	Intuition* FindIntution(std::string name);
	void AddIntuition(std::string name, Intuition* intuition);
};

extern IntuitionSystem intuitionSystem;

#include "IntuitionSystem.h"
#include <cassert>

IntuitionSystem intuitionSystem;

IntuitionSystem::IntuitionSystem() : System("IntuitionSystem")
{

}

Intuition* IntuitionSystem::FindIntution(std::string name)
{
	auto intuitionsIt = intuitions.find(name);
	assert(intuitionsIt != intuitions.end());
	return intuitionsIt->second;
}

void IntuitionSystem::AddIntuition(std::string name, Intuition* intuition)
{
	assert(intuition);
	intuition->name = name;
	intuitions.insert(std::make_pair(name, intuition));
}

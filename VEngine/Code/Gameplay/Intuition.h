#pragma once
#include <string>

//Base for all plyaer intuitions in-game.
struct Intuition
{
	std::string name;

	//Function to check any additional conditions or effect the Intuition might carry.
	virtual bool CheckEffect() = 0;
};

struct TestIntuition : Intuition
{
	virtual bool CheckEffect()
	{
		return true;
	}
};

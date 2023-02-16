#pragma once

#include <string>

//Base class for player in-game gear.
class PlayerGear
{
public:
	PlayerGear(std::string name_);
	virtual void Use() = 0;

private:
	std::string name;
};

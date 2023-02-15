#pragma once

#include <string>

//Base class for player in-game gear.
class PlayerGear
{
public:
	PlayerGear(std::string_view name_) : name(name_) {}
	virtual void Use() = 0;

private:
	std::string name;
};

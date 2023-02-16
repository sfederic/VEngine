#pragma once

#include <string>

//Base class for player in-game gear.
class Gear
{
public:
	Gear(std::string_view name_);
	virtual void Use() = 0;
	std::string GetName() { return name; }

private:
	std::string name;
};

#pragma once

#include <string>

enum class GearInputType
{
	Press,
	Hold
};

//Base class for player in-game gear.
class Gear
{
public:
	Gear(std::string_view name_);
	virtual void Use() = 0;

	auto GetName() { return name; }
	auto GetInputType() { return gearInputType; }

protected:
	GearInputType gearInputType = GearInputType::Press;
	std::string name;
};

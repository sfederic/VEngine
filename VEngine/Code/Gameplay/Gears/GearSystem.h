#pragma once

#include <map>
#include <string>

class PlayerGear;

class GearSystem
{
public:
	static GearSystem& Get()
	{
		static GearSystem instance;
		return instance;
	}

	void CreateGear(std::string gearName, PlayerGear* gear);
	PlayerGear* GetGear(const std::string gearName);

private:
	std::map<std::string, PlayerGear*> gearMap;
};

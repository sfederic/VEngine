#pragma once

#include <map>
#include <string>

class Gear;

class GearSystem
{
public:
	static GearSystem& Get()
	{
		static GearSystem instance;
		return instance;
	}

	void CreateGear(std::string gearName, Gear* gear);
	Gear* GetGear(const std::string gearName);
	std::vector<Gear*> GetAllGears();
	Gear* GetPrimaryGear();
	Gear* GetSecondaryGear();

private:
	std::map<std::string, Gear*> gearMap;
};

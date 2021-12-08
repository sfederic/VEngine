#pragma once
#include <string>

//This is a struct to copy data into to when spawning pickups across levels.
//@Todo: Engine doesn't have actor templates/prefabs/blueprints so this'll do for now.
struct PickupSpawnData
{
	std::string meshFilename;

	PickupSpawnData() {}
	PickupSpawnData(std::string meshFilename_)
	{
		meshFilename = meshFilename_;
	}
};

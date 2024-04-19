#pragma once

#include <string>

//This is to throw onto Materials to pool from a selection of similar sounds based on the type.
//For example, a Wooden type material, when recognised as such from whatever check, would use this type
//to play a random wood sound for footsteps or collisions.
struct MaterialAudioType
{
	inline static const std::string Default = "Default";
	inline static const std::string Metal = "Metal";
	inline static const std::string Grass = "Grass";
	inline static const std::string Wood = "Wood";
};

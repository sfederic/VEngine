#pragma once

#include <unordered_map>
#include <typeindex>
#include <optional>
#include <assert.h>
#include <string>
#include <fstream>
#include "Properties.h"

//Level save text file structure looks like this:
//ActorSystem Name
//Num of actors to spawn
//Actor Property - eg. Position
//Property value - eg. 1 -4 69.5
//next

struct Serialiser
{
	Serialiser(const std::string& file, std::ios_base::openmode mode);

	//TODO: I don't like these being static here.
	static void Serialise(Properties& props, std::ostream& os);
	static void Deserialise(Properties& props, std::istream& is);

	std::filebuf fb;
};

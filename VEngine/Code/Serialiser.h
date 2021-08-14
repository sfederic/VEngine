#pragma once
#include <fstream>
#include <string>

struct Properties;

struct Serialiser
{
	std::filebuf fb;

	Serialiser(const std::string& file, std::ios_base::openmode mode);
	void Serialise(const Properties& props, std::ostream& os);
	void Deserialise(const Properties& props, std::istream& is);
};

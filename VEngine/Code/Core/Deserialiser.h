#pragma once

#include <fstream>
#include <unordered_map>
#include "Core/OpenMode.h"

struct Properties;

class Deserialiser
{
private:
	//@Todo: there's a bad bug here with std::wifstream where if the textual type of of the property doesn't match
	//(e.g. a bool value is '205') then the whole program will loop infinitely. Maybe find a way to catch this.
	std::unordered_map<std::type_index, std::function<void(Property& prop)>> typeToReadFuncMap;

public:
	std::wifstream is;

	Deserialiser(const std::string filename, const OpenMode mode);
	~Deserialiser();
	void Deserialise(Properties& props);

	template <typename T>
	void ReadLine(T& arg)
	{
		is >> arg;
	}
};

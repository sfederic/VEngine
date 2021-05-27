#include "Serialise.h"
#include "Actor.h"

Serialiser::Serialiser(const std::string& file, std::ios_base::openmode mode)
{
	if (fb.is_open())
	{
		fb.close();
	}

	fb.open(file.c_str(), mode);
}

void Serialiser::Serialise(Actor* actor, std::ostream& os)
{
	Properties props = actor->GetProperties();
	for (auto& prop : props.dataMap)
	{
		std::type_index type = props.typeMap.find(prop.first)->second.value();

		if (type == typeid(float))
		{
			os << prop.first << ":" << *(float*)prop.second << "\n";
		}
		else if (type == typeid(bool))
		{
			os << prop.first << ":" << *(bool*)prop.second << "\n";
		}
		else if (type == typeid(std::string))
		{
			os << prop.first << ":" << (std::string&)prop.second << "\n";
		}
	}

	os << "next"; //"next" moves the forloop onto the next Actor in Deserialise
}

void Serialiser::Deserialise(Actor* actor, std::istream& is)
{
	Properties props = actor->GetProperties();

	char line[256];
	while (!is.eof())
	{
		is.getline(line, 256);

		std::string stringline = line;
		if (stringline.find("next") != stringline.npos) //Move to next actor
		{
			return;
		}

		char* value = nullptr;
		char* name = strtok_s(line, ":", &value);
		if (name == nullptr || value == nullptr || strcmp(value, "") == 0)
		{
			continue;
		}

		auto prop = props.dataMap.find(name);
		if (prop == props.dataMap.end())
		{
			continue;
		}

		std::type_index type = props.typeMap.find(name)->second.value();

		if (type == typeid(float))
		{
			*(float*)prop->second = std::stof(value);
		}
		else if (type == typeid(bool))
		{
			*(bool*)prop->second = std::stoi(value);
		}
		else if (type == typeid(std::string))
		{
			(std::string&)prop->second = value;
		}
	}
}

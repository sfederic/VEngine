#include "Serialise.h"
#include "Actor.h"

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
	}
}

void Serialiser::Deserialise(Actor* actor, FILE* file)
{
	Properties props = actor->GetProperties();

	char line[256];
	while (!feof(file))
	{
		fgets(line, 256, file);

		std::string stringline = line;
		if (stringline.find("ret") != stringline.npos) //Move to next actor (ret = return)
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
	}
}

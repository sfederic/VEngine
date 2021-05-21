#pragma once

#include <unordered_map>
#include <typeindex>
#include <optional>
#include <assert.h>
#include <string>
#include <fstream>

class Actor;

//Lot of good references and ideas on serialisation. Heaps of ideas on reflection, custom compile stages, header parsing.
//REF:https://google.github.io/flatbuffers/flatbuffers_guide_tutorial.html
//REF:http://donw.io/post/reflection-cpp-2/

//TODO: for this serialisation code to work, actorsystem needs to be updated with something similar to
/*void SerialiseAllActors(Serialiser& s, std::ostream& os)
{
	for (auto& actor : actors)
	{
		s.Serialise(actor, os);
		os << "ret\n";
	}
}

void DeserialiseAllActors(Serialiser& s, FILE* file)
{
	for (auto& actor : actors)
	{
		s.Deserialise(actor, file);
	}
}*/

//Container for storing variable data and variable types against a name on registration.
//Used with Actors during serialisation and creating property widgets.
//Properties are created in GetProps() function blocks so that the maps can be cascaded down
//actor inheritence trees, meaning you have to define Properties by hand in that same function, then
//call it when needed. Maybe look into some way to cache it. Performance wasn't a huge issue in testing.
struct Properties
{
	template <typename T>
	void Register(const std::string& name, T* data)
	{
		dataMap[name] = data;
		typeMap[name] = typeid(T);
	}

	void* GetData(const std::string& dataName)
	{
		auto& dataMapIt = dataMap.find(dataName);
		if(dataMapIt != dataMap.end())
		{
			return dataMapIt->second;
		}

		return nullptr; //Want to be able to fail finding keys when serialising.
	}

	std::type_index GetType(const std::string& typeName)
	{
		auto& typeMapIt = typeMap.find(typeName);
		return typeMapIt->second.value(); //Type will usually be grabbed after data, failing here acts as a check.
	}

	std::unordered_map<std::string, void*> dataMap;
	std::unordered_map<std::string, std::optional<std::type_index>> typeMap;
};

//TODO: fix these two functions up. They're a conjoined mess of C and C++ I/O libs
struct Serialiser
{
	void Serialise(Actor* actor, std::ostream& os);
	void Deserialise(Actor* actor, FILE* file);

	std::filebuf fb;
};

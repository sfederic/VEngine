#pragma once

#include <unordered_map>

struct Properties
{
	template <typename T>
	void Register(const char* name, T* value)
	{
		propertyMap[typeid(T)] = std::make_pair(name, (void*)&value);
	}

	std::unordered_map<std::type_index, std::pair<const char*, void*>> propertyMap;
};

template <typename T>
inline void put(const char* name, T value, FILE* file)
{
	fprintf(file "%s : %s\n", name, std::to_string(value).c_str());
}

inline void put(const char* name, const char* value, FILE* file)
{
	fprintf(file, "%s : %s\n", name, value);
}

template <typename T>
inline void get(const char* name, T* value, Properties& props, FILE* file)
{
	char strBuffer[256];
	fgets(strBuffer, 256, file);

	auto res = props.propertyMap.find()
	if (res != saveMap.end())
	{

	}

	if (str == nullptr)
	{
	}

	char* context = nullptr;
	auto tok = strtok_s(strBuffer, ":", &context);

	assert(context);
	if (typeid(T) == typeid(float)) {
		*value = std::stof(context);
	}
	else if (typeid(T) == typeid(int)) {
		*value = std::stoi(context);
	}
	else if (typeid(T) == typeid(bool)) {
		*value = std::stoi(context);
	}
}

#define PUT(value) put(#value, value);
#define GET(value) get(#value, &value);
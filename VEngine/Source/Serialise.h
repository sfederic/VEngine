#pragma once

#include <unordered_map>
#include <typeindex>
#include <optional>
#include <assert.h>
#include <string>

struct Properties
{
	template <typename T>
	void Register(const char* name, T* value)
	{
		dataMap[name] = &value;
		typeMap[name] = typeid(T);
	}

	void* GetData(const char* dataName)
	{
		auto dataMapIt = dataMap.find(dataName);
		return dataMapIt->second;
	}

	std::type_index GetType(const char* typeName)
	{
		auto typeMapIt = typeMap.find(typeName);
		return typeMapIt->second.value();
	}

	std::unordered_map<const char*, void*> dataMap;
	//There's a lot of fucking around with type_index if std::optional isn't here.
	std::unordered_map<const char*, std::optional<std::type_index>> typeMap;
};

template <typename T>
inline void Serialise(const char* name, T value, FILE* file)
{
	fprintf(file "%s : %s\n", name, std::to_string(value).c_str());
}

inline void Serialise(const char* name, const char* value, FILE* file)
{
	fprintf(file, "%s : %s\n", name, value);
}

template <typename T>
inline void Deserialise(const char* name, T* value, Properties& props, FILE* file)
{
	char strBuffer[256];
	fgets(strBuffer, 256, file);

	char* context = nullptr;
	auto tok = strtok_s(strBuffer, ":", &context);
	assert(context);

	void* data = props.GetData(context);
	if (data)
	{
		auto& type = props.GetType(dataMapIt.first);

		if (type == typeid(float)) {
			*value = std::stof(context);
		}
		else if (type == typeid(int)) {
			*value = std::stoi(context);
		}
		else if (type == typeid(bool)) {
			*value = std::stoi(context);
		}
	}
}

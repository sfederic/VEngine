#pragma once
#include <unordered_map>
#include <optional>
#include <string>
#include <typeindex>

struct Properties
{
	//PROPERTIES FOR TEXT
	std::unordered_map<std::string, void*> dataMap;
	std::unordered_map<std::string, std::optional<std::type_index>> typeMap;

	template <typename T>
	void Add(std::string name, T* data)
	{
		dataMap[name] = data;
		typeMap[name] = typeid(T);
	}

	template <typename T>
	T* GetData(const std::string& dataName)
	{
		auto dataMapIt = dataMap.find(dataName);
		if (dataMapIt != dataMap.end())
		{
			return static_cast<T*>(dataMapIt->second);
		}

		return nullptr;
	}

	std::type_index GetType(const std::string& typeName)
	{
		auto typeMapIt = typeMap.find(typeName);
		return typeMapIt->second.value();
	}

	////PROPERTIES FOR BINARY
	//std::unordered_map<size_t, void*> dataMap;
	//std::unordered_map<size_t, std::optional<std::type_index>> typeMap;
	// 
	//template <typename T>
	//void Add(const std::string& name, T* data)
	//{
	//	size_t hash = std::hash < std::string>{}(name);
	//	dataMap[hash] = data;
	//	typeMap[hash] = typeid(T);
	//}

	//void* GetData(size_t hash)
	//{
	//	auto& dataMapIt = dataMap.find(hash);
	//	if (dataMapIt != dataMap.end())
	//	{
	//		return dataMapIt->second;
	//	}

	//	return nullptr;
	//}

	//std::type_index GetType(size_t hash)
	//{
	//	auto& typeMapIt = typeMap.find(hash);
	//	return typeMapIt->second.value();
	//}
};

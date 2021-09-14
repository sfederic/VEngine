#pragma once
#include <map>
#include <optional>
#include <string>
#include <typeindex>

struct Property
{
	void* data = nullptr;
	//uint64_t offset = 0;
	uint64_t size = 0;
	std::optional<std::type_index> info;
	std::string name;
};

struct Properties
{
	//PROPERTIES FOR TEXT
	std::map<std::string, Property> propMap;
	std::string title;

	Properties() {}

	Properties(const char* title_)
	{
		title = title_;
	}

	template <typename T>
	void Add(std::string name, T* data)
	{
		Property prop = {};

		if (typeid(T) == typeid(std::string)) 
		{
			std::string* str = (std::string*)data;
			prop.size = str->size();
		} 
		else 
		{
			prop.size = sizeof(T);
		}

		prop.info = typeid(T);
		prop.name = name;
		prop.data = data;

		//If you ever need the offset for a property, you can do it where you pass in the Actor/Object
		//as a 'this' pointer through a template param. eg. <typename P> P* = this. Might help for serialisation?
		//prop.offset = ((uint64_t*)data - (uint64_t*)parent) * sizeof(T);

		assert(propMap.find(name) == propMap.end() && "name key already exists in properties map");

		propMap[name] = prop;
	}

	void Merge(Properties propsToMerge)
	{
		propMap.merge(propsToMerge.propMap);
	}

	template <typename T>
	bool CheckType(std::string name)
	{
		return propMap[name].info == typeid(T);
	}

	template <typename T>
	T* GetData(std::string name)
	{
		return (T*)propMap[name].data;
	}

	std::type_index GetType(std::string name)
	{
		return propMap[name].info.value();
	}

	//Used to copy data in from other Properties (types have to be matching)
	void CopyData(std::string name, void* dataToCopy)
	{
		Property& prop = propMap[name];

		if (prop.info == typeid(std::string))
		{
			std::string* dst = (std::string*)prop.data;
			std::string* src = (std::string*)dataToCopy;

			dst->assign(src->c_str());
		}
		else
		{
			memcpy(prop.data, dataToCopy, prop.size);
		}
	}

	//PROPERTIES FOR BINARY 
	//Binary props work the same as text, just hash the name passed in
	//template <typename T>
	//void Add(const std::string& name, T* data)
	//{
	//	size_t hash = std::hash<std::string>{}(name);
	//}
};

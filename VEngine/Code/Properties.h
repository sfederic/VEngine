#pragma once
#include <map>
#include <vector>
#include "Property.h"
#include "Render/RenderTypes.h"

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
	Property& Add(std::string name, T* data)
	{
		Property prop = {};

		if (typeid(T) == typeid(std::string)) 
		{
			auto str = (std::string*)data;
			prop.size = str->size() + 1; //'+ 1' is for null terminator '\0'
		} 
		else if (typeid(T) == typeid(std::wstring))
		{
			auto str = (std::wstring*)data;
			prop.size = (str->size() * 2) + 1; //Double size for wstring
		}
		else if (typeid(T) == typeid(ShaderData))
		{
			auto shaderData = (ShaderData*)data;
			prop.size = shaderData->filename.size() + 1;
		}
		else if (typeid(T) == typeid(TextureData))
		{
			auto textureData = (TextureData*)data;
			prop.size = textureData->filename.size() + 1;
		}
		else if (typeid(T) == typeid(MeshComponentData))
		{
			//MeshComponentData has a pointer to a MeshComponent in it too,
			//but serialising it only deals with the string so it's alright. Might fuck up on binary.
			auto meshComponentData = (MeshComponentData*)data;
			prop.size = meshComponentData->filename.size() + 1;
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

		return propMap[name];
	}

	//Shorthand #define for adding variable pointer and name.
#define AddProp(x) Add(#x, &x)

	void Merge(Properties propsToMerge)
	{
		propMap.merge(propsToMerge.propMap);
	}

	template <typename T>
	bool CheckType(std::string name)
	{
		return propMap[name].info.value() == typeid(T);
	}

	template <typename Base, typename Derived>
	bool CheckTypeDynamicCast(const std::string name)
	{
		auto derived = (Derived*)propMap[name].data;
		return dynamic_cast<Base*>(derived);
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

	Property* GetProperty(std::string name)
	{
		return &propMap[name];
	}

	//Used to copy data in from other Properties (types have to be matching)
	void CopyData(std::string name, Property& propToCopy);

	//Copy matching vectors of properties across (currently only works for exact matching properties collections,
	//should be able to work with partial matching collections though without the vectors).
	static void CopyProperties(std::vector<Properties>& src, std::vector<Properties>& dst);

	//template <typename T>
	//void Add(const std::string& name, T* data)
	//{
	//	size_t hash = std::hash<std::string>{}(name);
	//}
};

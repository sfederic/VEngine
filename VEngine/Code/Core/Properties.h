#pragma once

#include <map>
#include <vector>
#include "Property.h"
#include "Render/RenderPropertyStructs.h"

struct Properties
{
	//PROPERTIES FOR TEXT
	std::map<std::string, Property> propMap;
	std::string title;

	UID ownerUID = 0;

	Properties() {}

	Properties(const char* title_)
	{
		title = title_;
	}

	template <typename T>
	Property& Add(const std::string& name, T* data)
	{
		Property prop = {};

		if (typeid(T) == typeid(std::string)) 
		{
			auto str = reinterpret_cast<std::string*>(data);
			prop.size = str->size();
		} 
		else if (typeid(T) == typeid(std::wstring))
		{
			auto str = reinterpret_cast<std::wstring*>(data);
			prop.size = str->size() * sizeof(wchar_t);
		}
		else if (typeid(T) == typeid(ShaderData))
		{
			auto shaderData = reinterpret_cast<ShaderData*>(data);
			prop.size = shaderData->shaderItemName.size() * sizeof(char);
		}
		else if (typeid(T) == typeid(TextureData))
		{
			auto textureData = reinterpret_cast<TextureData*>(data);
			prop.size = textureData->filename.size() * sizeof(char);
		}
		else if (typeid(T) == typeid(MeshComponentData))
		{
			auto meshComponentData = reinterpret_cast<MeshComponentData*>(data);
			prop.size = meshComponentData->filename.size() * sizeof(char);
		}
		else 
		{
			prop.size = sizeof(T);
		}

		prop.info = typeid(T);
		prop.name = name;
		prop.data = data;
		prop.ownerUID = ownerUID;

		//If you ever need the offset for a property, you can do it where you pass in the Actor/Object
		//as a 'this' pointer through a template param. eg. <typename P> P* = this. Might help for serialisation?
		//prop.offset = ((uint64_t*)data - (uint64_t*)parent) * sizeof(T);

		assert(propMap.find(name) == propMap.end() && "name key already exists in properties map");

		propMap.emplace(name, prop);

		return propMap.find(name)->second;
	}

	void Merge(Properties& propsToMerge)
	{
		propMap.merge(propsToMerge.propMap);
	}

	//For properties that aren't from an actor or component, but fetched from within those GetProps() (e.g. Material)
	void SetAllPropertyOwnerUIDs(UID ownerUID_)
	{
		ownerUID = ownerUID_;

		for (auto& [name, prop] : propMap)
		{
			prop.ownerUID = ownerUID;
		}
	}

	template <typename T>
	bool CheckType(const std::string& name)
	{
		assert(propMap.find(name) != propMap.end());
		return propMap.find(name)->second.info.value() == typeid(T);
	}

	template <typename T>
	T* GetData(const std::string& name)
	{
		assert(propMap.find(name) != propMap.end());
		return reinterpret_cast<T*>(propMap.find(name)->second.data);
	}

	template <typename T>
	T* GetDataAllowNull(const std::string& name)
	{
		auto dataIt = propMap.find(name);
		if (dataIt != propMap.end())
		{
			return reinterpret_cast<T*>(dataIt->second.data);
		}
		return nullptr;
	}

	std::type_index GetType(const std::string& name)
	{
		assert(propMap.find(name) != propMap.end());
		return propMap.find(name)->second.info.value();
	}

	Property* GetProperty(const std::string& name)
	{
		assert(propMap.find(name) != propMap.end());
		return &propMap.find(name)->second;
	}

	//Used to copy data in from other Properties (types have to be matching)
	void CopyData(const std::string& name, Property& propToCopy);

	//Copy matching vectors of properties across (currently only works for exact matching properties collections,
	//should be able to work with partial matching collections though without the vectors).
	static void CopyProperties(std::vector<Properties>& src, std::vector<Properties>& dst);
};

#pragma once

#include <map>
#include <vector>
#include <string>
#include <typeindex>
#include <cassert>
#include "Property.h"
#include "Render/RenderPropertyStructs.h"

struct Properties
{
	std::string title;
	std::map<std::string, Property> propMap;
	UID ownerUID = 0;

	Properties() = default;

	explicit Properties(const char* title_) : title(title_) {}

	bool Find(const std::string& name) const
	{
		return propMap.find(name) != propMap.end();
	}

	template <typename T>
	Property& Add(const std::string& name, T* data)
	{
		assert(propMap.find(name) == propMap.end() && "name key already exists in properties map");

		Property prop;
		SetPropertySize<T>(prop, data);
		prop.info = typeid(T);
		prop.name = name;
		prop.data = data;
		prop.ownerUID = ownerUID;

		return propMap.emplace(prop.name, std::move(prop)).first->second;
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
		return propMap.find(name)->second.info.value() == typeid(T);
	}

	template <typename T>
	T* GetData(const std::string& name)
	{
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
		return propMap.find(name)->second.info.value();
	}

	Property* GetProperty(const std::string& name)
	{
		return &propMap.find(name)->second;
	}

	//Used to copy data in from other Properties (types have to be matching)
	void CopyData(const std::string& name, Property& propToCopy);

	//Copy matching vectors of properties across (currently only works for exact matching properties collections,
	//should be able to work with partial matching collections though without the vectors).
	static void CopyProperties(std::vector<Properties>& src, std::vector<Properties>& dst);

private:
	template <typename T>
	void SetPropertySize(Property& prop, T* data)
	{
		if constexpr (std::is_same_v<T, std::string>)
		{
			prop.size = data->size();
		}
		else if constexpr (std::is_same_v<T, std::wstring>)
		{
			prop.size = data->size() * sizeof(wchar_t);
		}
		else if constexpr (std::is_same_v<T, TextureData>)
		{
			prop.size = data->filename.size();
		}
		else if constexpr (std::is_same_v<T, MeshComponentData>)
		{
			prop.size = data->filename.size();
		}
		else
		{
			prop.size = sizeof(T);
		}
	}
};

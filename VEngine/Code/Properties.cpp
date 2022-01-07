#include "Properties.h"

void Properties::CopyData(std::string name, Property& propToCopy)
{
	Property& prop = propMap[name];

	assert(prop.info == propToCopy.info && "property types not matching");

	if (prop.info == typeid(std::string))
	{
		auto dst = (std::string*)prop.data;
		auto src = (std::string*)propToCopy.data;

		*dst = src->c_str();
	}
	else if (prop.info == typeid(std::wstring))
	{
		auto dst = (std::wstring*)prop.data;
		auto src = (std::wstring*)propToCopy.data;

		*dst = src->c_str();
	}
	else if (prop.info == typeid(ShaderData))
	{
		auto dst = (ShaderData*)prop.data;
		auto src = (ShaderData*)propToCopy.data;

		dst->filename = src->filename.c_str();
	}
	else if (prop.info == typeid(TextureData))
	{
		auto dst = (TextureData*)prop.data;
		auto src = (TextureData*)propToCopy.data;

		dst->filename = src->filename.c_str();
	}
	else if (prop.info == typeid(MeshComponentData))
	{
		auto dst = (MeshComponentData*)prop.data;
		auto src = (MeshComponentData*)propToCopy.data;

		dst->filename = src->filename.c_str();
	}
	else
	{
		assert(prop.size == propToCopy.size);
		memcpy(prop.data, propToCopy.data, prop.size);
	}
}

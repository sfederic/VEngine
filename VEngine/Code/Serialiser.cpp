#include "Serialiser.h"
#include <DirectXMath.h>
#include <typeindex>
#include "Properties.h"

using namespace DirectX;

Serialiser::Serialiser(const std::string& filename, OpenMode openMode) : os(&fb), is(&fb)
{
	fb.open(filename.c_str(), (std::ios_base::openmode)openMode);
}

void Serialiser::Serialise(Properties props)
{
	for (auto prop : props.propMap)
	{
		const std::string& name = prop.first;

		if (props.CheckType<float>(name))
		{
			os << name << "\n" << *props.GetData<float>(name) << "\n";
		}
		else if (props.CheckType<XMFLOAT2>(name))
		{
			XMFLOAT2* value = props.GetData<XMFLOAT2>(name);
			os << name << "\n" << value->x << " " << value->y << "\n";
		}		
		else if (props.CheckType<XMFLOAT3>(name))
		{
			XMFLOAT3* value = props.GetData<XMFLOAT3>(name);
			os << name << "\n" << value->x << " " << value->y << " " << value->z << "\n";
		}
		else if (props.CheckType<XMFLOAT4>(name))
		{
			XMFLOAT4* value = props.GetData<XMFLOAT4>(name);
			os << name << "\n" << value->x << " " << value->y << " " << value->z << " " << value->w << "\n";
		}
		else if (props.CheckType<bool>(name))
		{
			os << name << "\n" << *props.GetData<bool>(name) << "\n";
		}
		else if (props.CheckType<int>(name))
		{
			os << name << "\n" << *props.GetData<int>(name) << "\n";
		}
		else if (props.CheckType<std::string>(name))
		{
			std::string* str = props.GetData<std::string>(name);
			os << name << "\n" << str->c_str() << "\n";
		}
	}

	os << "next\n"; //"next" moves the forloop onto the next 'Object'
}

void Serialiser::Deserialise(Properties props)
{
	char line[512];
	while (!is.eof())
	{
		is.getline(line, 512);

		std::string stringline = line;
		if (stringline.find("next") != stringline.npos) //Move to next Object
		{
			return;
		}

		auto prop = props.propMap.find(line);
		if (prop == props.propMap.end())
		{
			continue;
		}

		const std::string& name = prop->first;

		if (props.CheckType<float>(name))
		{
			is >> *props.GetData<float>(name);
		}
		else if (props.CheckType<XMFLOAT2>(name))
		{
			XMFLOAT2* float3 = props.GetData<XMFLOAT2>(name);
			is >> float3->x;
			is >> float3->y;
		}		
		else if (props.CheckType<XMFLOAT3>(name))
		{
			XMFLOAT3* float3 = props.GetData<XMFLOAT3>(name);
			is >> float3->x;
			is >> float3->y;
			is >> float3->z;
		}
		else if (props.CheckType<XMFLOAT4>(name))
		{
			XMFLOAT4* float4 = props.GetData<XMFLOAT4>(name);
			is >> float4->x;
			is >> float4->y;
			is >> float4->z;
			is >> float4->w;
		}
		else if (props.CheckType<bool>(name))
		{
			is >> *props.GetData<bool>(name);
		}
		else if (props.CheckType<int>(name))
		{
			is >> *props.GetData<int>(name);
		}
		else if (props.CheckType<std::string>(name))
		{
			char propString[512];
			is.getline(propString, 512);
			std::string* str = props.GetData<std::string>(name);
			str->assign(propString);
		}
	}
}

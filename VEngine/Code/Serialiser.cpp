#include "vpch.h"
#include "Serialiser.h"
#include "Properties.h"
#include "Actors/Actor.h"
#include "World.h"
#include "Render/RenderTypes.h"
#include "VString.h"
#include "VEnum.h"

using namespace DirectX;

std::unordered_map<std::type_index, std::function<void(Property& prop, std::wstring& name)>> typeToWriteFuncMap;

void BinarySerialiser::Serialise(Properties& props)
{
	for (auto& propPair : props.propMap)
	{
		auto& prop = propPair.second;

		if (props.CheckType<std::string>(propPair.first))
		{
			auto str = (std::string*)prop.data;
			if (fwrite(str->data(), sizeof(char), prop.size, file) == 0)
			{
				throw;
			}
		}
		else if (props.CheckType<std::wstring>(propPair.first))
		{
			auto wstr = (std::wstring*)prop.data;
			if (fwrite(wstr->data(), sizeof(wchar_t), prop.size, file) == 0)
			{
				throw;
			}
		}
		else if (props.CheckType<MeshComponentData>(propPair.first) ||
			props.CheckType<TextureData>(propPair.first) ||
			props.CheckType<ShaderData>(propPair.first))
		{
			//Just serialise out strings for Render Data types
			auto str = (std::string*)prop.data;
			if (fwrite(str->data(), sizeof(char), str->size() + 1, file) == 0)
			{
				throw;
			}
		}
		else
		{
			if (fwrite(prop.data, prop.size, 1, file) == 0)
			{
				throw;
			}
		}
	}
}

void BinaryDeserialiser::Deserialise(Properties& props)
{
	for (auto& propPair : props.propMap)
	{
		auto& prop = propPair.second;

		if (props.CheckType<std::string>(propPair.first))
		{
			auto str = props.GetData<std::string>(propPair.first);
			if (fread(str->data(), sizeof(char), prop.size, file) == 0)
			{
				throw;
			}
		}
		else if (props.CheckType<std::wstring>(propPair.first))
		{
			auto str = props.GetData<std::wstring>(propPair.first);
			if (fread(str->data(), sizeof(wchar_t), prop.size, file) == 0)
			{
				throw;
			}
		}
		else if (props.CheckType<MeshComponentData>(propPair.first) ||
			props.CheckType<TextureData>(propPair.first) ||
			props.CheckType<ShaderData>(propPair.first))
		{
			auto wstr = props.GetData<std::wstring>(propPair.first);
			if (fread(wstr->data(), sizeof(wchar_t), (wstr->size() * 2) + 1, file) == 0)
			{
				throw;
			}
		}
		else
		{
			if (fread(prop.data, prop.size, 1, file) == 0)
			{
				throw;
			}
		}
	}
}

Serialiser::Serialiser(const std::string filename_, const OpenMode mode_) :
	filename(filename_), mode(mode_)
{
	typeToWriteFuncMap[typeid(bool)] = [&](Property& prop, std::wstring& name) {
		ss << name << "\n" << *prop.GetData<bool>() << "\n"; 
	};

	typeToWriteFuncMap[typeid(float)] = [&](Property& prop, std::wstring& name) {
		ss << name << "\n" << *prop.GetData<float>() << "\n";
	};

	typeToWriteFuncMap[typeid(XMFLOAT2)] = [&](Property& prop, std::wstring& name) {
		auto value = prop.GetData<XMFLOAT2>();
		ss << name << "\n" << value->x << " " << value->y << "\n";
	};

	typeToWriteFuncMap[typeid(XMFLOAT3)] = [&](Property& prop, std::wstring& name) {
		auto value = prop.GetData<XMFLOAT3>();
		ss << name << "\n" << value->x << " " << value->y << " " << value->z << "\n"; 
	};

	typeToWriteFuncMap[typeid(XMFLOAT4)] = [&](Property& prop, std::wstring& name) {
		auto value = prop.GetData<XMFLOAT4>();
		ss << name << "\n" << value->x << " " << value->y << " " << value->z << " " << value->w << "\n";
	};

	typeToWriteFuncMap[typeid(int)] = [&](Property& prop, std::wstring& name) {
		ss << name << "\n" << *prop.GetData<int>() << "\n";
	};

	typeToWriteFuncMap[typeid(std::string)] = [&](Property& prop, std::wstring& name) {
		auto str = prop.GetData<std::string>();
		ss << name << "\n" << str->c_str() << "\n";
	};

	typeToWriteFuncMap[typeid(std::wstring)] = [&](Property& prop, std::wstring& name) {
		auto wstr = prop.GetData<std::wstring>();
		ss << name << "\n" << VString::wstos(wstr->data()).c_str() << "\n";
	};

	typeToWriteFuncMap[typeid(TextureData)] = [&](Property& prop, std::wstring& name) {
		auto textureData = prop.GetData<TextureData>();
		ss << name << "\n" << textureData->filename.c_str() << "\n";
	};

	typeToWriteFuncMap[typeid(std::string)] = [&](Property& prop, std::wstring& name) {
		auto shaderData = prop.GetData<ShaderData>();
		ss << name << "\n" << shaderData->filename.c_str() << "\n";
	};

	typeToWriteFuncMap[typeid(MeshComponentData)] = [&](Property& prop, std::wstring& name) {
		auto meshComponentData = prop.GetData<MeshComponentData>();
		ss << name << "\n" << meshComponentData->filename.c_str() << "\n";
	};

	typeToWriteFuncMap[typeid(ShaderData)] = [&](Property& prop, std::wstring& name) {
		auto shaderData = prop.GetData<ShaderData>();
		ss << name << "\n" << shaderData->filename.c_str() << "\n";
	};

	typeToWriteFuncMap[typeid(UID)] = [&](Property& prop, std::wstring& name) {
		auto uid = prop.GetData<UID>();
		ss << name << "\n";
		ss << *uid << "\n";
	};

	typeToWriteFuncMap[typeid(VEnum)] = [&](Property& prop, std::wstring& name) {
		auto vEnum = prop.GetData<VEnum>();
		ss << name << "\n";
		ss << vEnum->GetValue().c_str() << "\n";
	};
}

Serialiser::~Serialiser()
{
	//A bit counter-intuitive to put file opening for the stringstream in destructor,
	//but otherwise the entire file is overwritten from the start, meaning you miss out 
	//on the safety stringstream is lending as a temp buffer during crashes.
	ofs.open(filename.c_str(), (std::ios_base::openmode)mode);
	assert(!ofs.fail());

	ofs << ss.str();

	ofs.flush();
	ofs.close();
}

void Serialiser::Serialise(Properties& props)
{
	for (auto& propPair : props.propMap)
	{
		std::wstring wname = VString::stows(propPair.first);
		Property& prop = propPair.second;

		auto typeIt = typeToWriteFuncMap.find(prop.info.value());
		assert(typeIt != typeToWriteFuncMap.end() && "Matching type_info won't be found in map");
		typeIt->second(prop, wname);
	}

	ss << "next\n"; //"next" moves the forloop onto the next 'Object'
}

Deserialiser::Deserialiser(const std::string filename, const OpenMode mode)
{
	is.open(filename.c_str(), (std::ios_base::openmode)mode);
	if (is.fail())
	{
		throw;
	}

	//@Todo: with wstrings for .vmap text files, reading in wstrings produces junk. Might not be a problem for bianary.
	//Ref:https://coderedirect.com/questions/456819/is-it-possible-to-set-a-text-file-to-utf-16
	//std::locale loc(std::locale::classic(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>);
	//ofs.imbue(loc);
}

Deserialiser::~Deserialiser()
{
	is.close();
}

void Deserialiser::Deserialise(Properties& props)
{
	wchar_t line[512];
	while (!is.eof())
	{
		is.getline(line, 512);

		std::wstring stringline = line;
		if (stringline.find(L"next") != stringline.npos) //Move to next Object
		{
			return;
		}

		auto prop = props.propMap.find(VString::wstos(line));
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
			XMFLOAT2* float2 = props.GetData<XMFLOAT2>(name);
			is >> float2->x;
			is >> float2->y;
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
			wchar_t propString[512];
			is.getline(propString, 512);
			std::string* str = props.GetData<std::string>(name);
			str->assign(VString::wstos(propString));
		}
		else if (props.CheckType<std::wstring>(name))
		{
			//wstring is converted to string on Serialise. Convert back to wstring here.
			wchar_t propString[512];
			is.getline(propString, 512);
			std::wstring* wstr = props.GetData<std::wstring>(name);
			wstr->assign(propString);
		}
		else if (props.CheckType<TextureData>(name))
		{
			wchar_t propString[512];
			is.getline(propString, 512);
			TextureData* textureData = props.GetData<TextureData>(name);
			textureData->filename.assign(VString::wstos(propString));
		}
		else if (props.CheckType<ShaderData>(name))
		{
			wchar_t propString[512]{};
			is.getline(propString, 512);
			ShaderData* shaderData = props.GetData<ShaderData>(name);
			shaderData->filename.assign(VString::wstos(propString));
		}		
		else if (props.CheckType<MeshComponentData>(name))
		{
			wchar_t propString[512]{};
			is.getline(propString, 512);
			MeshComponentData* meshComponentData = props.GetData<MeshComponentData>(name);
			meshComponentData->filename.assign(VString::wstos(propString));
		}
		else if (props.CheckType<UID>(name))
		{
			UID* uid = props.GetData<UID>(name);
			is >> *uid;
		}
		else if (props.CheckType<VEnum>(name))
		{
			wchar_t propString[512]{};
			is.getline(propString, 512);
			auto vEnum = props.GetData<VEnum>(name);
			vEnum->SetValue(VString::wstos(propString));
		}
	}
}

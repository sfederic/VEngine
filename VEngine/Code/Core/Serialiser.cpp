#include "vpch.h"
#include "Serialiser.h"
#include "Properties.h"
#include "Render/RenderPropertyStructs.h"
#include "VString.h"
#include "VEnum.h"

using namespace DirectX;

void BinarySerialiser::Serialise(Properties& props)
{
	for (const auto& [name, prop] : props.propMap)
	{
		if (props.CheckType<std::string>(name))
		{
			auto str = props.GetData<std::string>(name);
			WriteString(*str);
		}
		else if (props.CheckType<std::wstring>(name))
		{
			auto wstr = props.GetData<std::wstring>(name);
			WriteWString(*wstr);
		}
		else if (props.CheckType<MeshComponentData>(name))
		{
			auto meshComponentData = props.GetData<MeshComponentData>(name);
			WriteString(meshComponentData->filename);
		}
		else if (props.CheckType<TextureData>(name))
		{
			auto textureData = props.GetData<TextureData>(name);
			WriteString(textureData->filename);
		}
		else if (props.CheckType<VEnum>(name))
		{
			auto vEnum = props.GetData<VEnum>(name);
			WriteString(vEnum->GetValue());
		}
		else
		{
			fwrite(prop.data, prop.size, 1, file);
		}
	}
}

void BinarySerialiser::WriteString(const std::string str)
{
	const size_t stringSize = str.length();
	fwrite(&stringSize, sizeof(size_t), 1, file);
	fwrite(str.c_str(), stringSize, 1, file);
}

void BinarySerialiser::WriteWString(const std::wstring wstr)
{
	const std::string str = VString::wstos(wstr);
	WriteString(str);
}

void BinaryDeserialiser::Deserialise(Properties& props)
{
	for (auto& [name, prop] : props.propMap)
	{
		if (props.CheckType<std::string>(name))
		{
			auto str = props.GetData<std::string>(name);
			ReadString(str);
		}
		else if (props.CheckType<std::wstring>(name))
		{
			auto str = props.GetData<std::wstring>(name);
			ReadWString(str);
		}
		else if (props.CheckType<MeshComponentData>(name))
		{
			auto meshComponentData = props.GetData<MeshComponentData>(name);
			ReadString(&meshComponentData->filename);
		}
		else if (props.CheckType<TextureData>(name))
		{
			auto textureData = props.GetData<TextureData>(name);
			ReadString(&textureData->filename);
		}
		else if (props.CheckType<VEnum>(name))
		{
			auto vEnum = props.GetData<VEnum>(name);
			std::string readStr;
			ReadString(&readStr);
			vEnum->SetValue(readStr);
		}
		else
		{
			fread(prop.data, prop.size, 1, file);
		}
	}
}

void BinaryDeserialiser::ReadString(std::string* str)
{
	size_t stringSize = 0;
	fread(&stringSize, sizeof(size_t), 1, file);

	std::vector<char> buff(stringSize);
	fread(buff.data(), stringSize, 1, file);
	assert(buff.size() == stringSize);

	const std::string newStr(buff.data(), stringSize);
	*str = newStr;
}

void BinaryDeserialiser::ReadWString(std::wstring* wstr)
{
	std::string str;
	ReadString(&str);
	*wstr = VString::stows(str);
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

	typeToWriteFuncMap[typeid(XMINT2)] = [&](Property& prop, std::wstring& name) {
		auto value = prop.GetData<XMINT2>();
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

	typeToWriteFuncMap[typeid(MeshComponentData)] = [&](Property& prop, std::wstring& name) {
		auto meshComponentData = prop.GetData<MeshComponentData>();
		ss << name << "\n" << meshComponentData->filename.c_str() << "\n";
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

	//Setup read map
	typeToReadFuncMap[typeid(float)] = [&](Property& prop) {
		is >> *prop.GetData<float>();
	};

	typeToReadFuncMap[typeid(XMFLOAT2)] = [&](Property& prop) {
		auto float2 = prop.GetData<XMFLOAT2>();
		is >> float2->x;
		is >> float2->y;
	};

	typeToReadFuncMap[typeid(XMINT2)] = [&](Property& prop) {
		auto int2 = prop.GetData<XMINT2>();
		is >> int2->x;
		is >> int2->y;
	};

	typeToReadFuncMap[typeid(XMFLOAT3)] = [&](Property& prop) {
		auto float3 = prop.GetData<XMFLOAT3>();
		is >> float3->x;
		is >> float3->y;
		is >> float3->z;
	};
	
	typeToReadFuncMap[typeid(XMFLOAT4)] = [&](Property& prop) {
		auto float4 = prop.GetData<XMFLOAT4>();
		is >> float4->x;
		is >> float4->y;
		is >> float4->z;
		is >> float4->w;
	};

	typeToReadFuncMap[typeid(bool)] = [&](Property& prop) {
		is >> *prop.GetData<bool>();
	};

	typeToReadFuncMap[typeid(int)] = [&](Property& prop) {
		is >> *prop.GetData<int>();
	};

	typeToReadFuncMap[typeid(std::string)] = [&](Property& prop) {
		wchar_t propString[512]{};
		is.getline(propString, 512);
		auto str = prop.GetData<std::string>();
		str->assign(VString::wstos(propString));
	};
		
	typeToReadFuncMap[typeid(std::wstring)] = [&](Property& prop) {
		//wstring is converted to string on Serialise. Convert back to wstring here.
		wchar_t propString[512]{};
		is.getline(propString, 512);
		auto str = prop.GetData<std::wstring>();
		str->assign(propString);
	};
	
	typeToReadFuncMap[typeid(TextureData)] = [&](Property& prop) {
		wchar_t propString[512]{};
		is.getline(propString, 512);
		auto textureData = prop.GetData<TextureData>();
		textureData->filename.assign(VString::wstos(propString));
	};

	typeToReadFuncMap[typeid(MeshComponentData)] = [&](Property& prop) {
		wchar_t propString[512]{};
		is.getline(propString, 512);
		auto meshComponentData = prop.GetData<MeshComponentData>();
		meshComponentData->filename.assign(VString::wstos(propString));
	};

	typeToReadFuncMap[typeid(UID)] = [&](Property& prop) {
		UID* uid = prop.GetData<UID>();
		is >> *uid;
	};

	typeToReadFuncMap[typeid(VEnum)] = [&](Property& prop) {
		wchar_t propString[512]{};
		is.getline(propString, 512);
		auto vEnum = prop.GetData<VEnum>();
		vEnum->SetValue(VString::wstos(propString));
	};
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

		auto propIt = props.propMap.find(VString::wstos(line));
		if (propIt == props.propMap.end())
		{
			continue;
		}

		const std::string& name = propIt->first;
		Property& prop = propIt->second;

		auto funcIt = typeToReadFuncMap.find(prop.info.value());
		assert(funcIt != typeToReadFuncMap.end() && "Matching type_info not found");
		auto& func = funcIt->second;
		func(prop);
	}
}

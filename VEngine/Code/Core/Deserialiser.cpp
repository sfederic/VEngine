#include "vpch.h"
#include "Deserialiser.h"
#include "Core/VEnum.h"
#include <locale>
#include <bit>
#include <codecvt>

using namespace DirectX;

Deserialiser::Deserialiser(const std::string filename, const OpenMode mode)
{
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::little_endian>);
	is.imbue(loc);

	is.open(filename.c_str(), (std::ios_base::openmode)mode);
	if (is.fail())
	{
		throw;
	}

	//Setup read map
	typeToReadFuncMap.emplace(typeid(float), [&](Property& prop) {
		is >> *prop.GetData<float>();
		});

	typeToReadFuncMap.emplace(typeid(XMFLOAT2), [&](Property& prop) {
		auto float2 = prop.GetData<XMFLOAT2>();
		is >> float2->x;
		is >> float2->y;
		});

	typeToReadFuncMap.emplace(typeid(XMINT2), [&](Property& prop) {
		auto int2 = prop.GetData<XMINT2>();
		is >> int2->x;
		is >> int2->y;
		});

	typeToReadFuncMap.emplace(typeid(XMFLOAT3), [&](Property& prop) {
		auto float3 = prop.GetData<XMFLOAT3>();
		is >> float3->x;
		is >> float3->y;
		is >> float3->z;
		});

	typeToReadFuncMap.emplace(typeid(XMFLOAT4), [&](Property& prop) {
		auto float4 = prop.GetData<XMFLOAT4>();
		is >> float4->x;
		is >> float4->y;
		is >> float4->z;
		is >> float4->w;
		});

	typeToReadFuncMap.emplace(typeid(bool), [&](Property& prop) {
		is >> *prop.GetData<bool>();
		});

	typeToReadFuncMap.emplace(typeid(int), [&](Property& prop) {
		is >> *prop.GetData<int>();
		});

	typeToReadFuncMap.emplace(typeid(std::string), [&](Property& prop) {
		wchar_t propString[512]{};
		is.getline(propString, 512);
		auto str = prop.GetData<std::string>();
		str->assign(VString::wstos(propString));
		});

	typeToReadFuncMap.emplace(typeid(std::wstring), [&](Property& prop) {
		wchar_t propString[512]{};
		is.getline(propString, 512);
		auto str = prop.GetData<std::wstring>();
		str->assign(propString);
		});

	typeToReadFuncMap.emplace(typeid(TextureData), [&](Property& prop) {
		wchar_t propString[512]{};
		is.getline(propString, 512);
		auto textureData = prop.GetData<TextureData>();
		textureData->filename.assign(VString::wstos(propString));
		});

	typeToReadFuncMap.emplace(typeid(MeshComponentData), [&](Property& prop) {
		wchar_t propString[512]{};
		is.getline(propString, 512);
		auto meshComponentData = prop.GetData<MeshComponentData>();
		meshComponentData->filename.assign(VString::wstos(propString));
		});

	typeToReadFuncMap.emplace(typeid(UID), [&](Property& prop) {
		UID* uid = prop.GetData<UID>();
		is >> *uid;
		});

	typeToReadFuncMap.emplace(typeid(VEnum), [&](Property& prop) {
		wchar_t propString[512]{};
		is.getline(propString, 512);
		auto vEnum = prop.GetData<VEnum>();
		vEnum->SetValue(VString::wstos(propString));
		});
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

		const std::wstring wStrLine = line;

		if (wStrLine.empty())
		{
			continue;
		}

		if (wStrLine.find(L"next") != wStrLine.npos) //Move to next Object
		{
			return;
		}

		const std::string strLine = VString::wstos(wStrLine);
		auto propIt = props.propMap.find(strLine);
		if (propIt == props.propMap.end())
		{
			continue;
		}

		Property& prop = propIt->second;

		auto funcIt = typeToReadFuncMap.find(prop.info.value());
		assert(funcIt != typeToReadFuncMap.end() && "Matching type_info not found");
		auto& func = funcIt->second;
		func(prop);
	}
}

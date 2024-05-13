#include "vpch.h"
#include "Serialiser.h"
#include "Properties.h"
#include "Render/RenderPropertyStructs.h"
#include "VString.h"
#include "VEnum.h"
#include <locale>
#include <bit>
#include <codecvt>

using namespace DirectX;

Serialiser::Serialiser(const std::string filename_, const OpenMode mode_) :
	filename(filename_), mode(mode_)
{
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::little_endian>);
	ofs.imbue(loc);

	typeToWriteFuncMap.emplace(typeid(bool), [&](Property& prop, std::wstring& name) {
		ss << name << "\n" << *prop.GetData<bool>() << "\n";
		});

	typeToWriteFuncMap.emplace(typeid(float), [&](Property& prop, std::wstring& name) {
		ss << name << "\n" << *prop.GetData<float>() << "\n";
		});

	typeToWriteFuncMap.emplace(typeid(XMFLOAT2), [&](Property& prop, std::wstring& name) {
		auto value = prop.GetData<XMFLOAT2>();
		ss << name << "\n" << value->x << " " << value->y << "\n";
		});

	typeToWriteFuncMap.emplace(typeid(XMINT2), [&](Property& prop, std::wstring& name) {
		auto value = prop.GetData<XMINT2>();
		ss << name << "\n" << value->x << " " << value->y << "\n";
		});

	typeToWriteFuncMap.emplace(typeid(XMFLOAT3), [&](Property& prop, std::wstring& name) {
		auto value = prop.GetData<XMFLOAT3>();
		ss << name << "\n" << value->x << " " << value->y << " " << value->z << "\n";
		});

	typeToWriteFuncMap.emplace(typeid(XMFLOAT4), [&](Property& prop, std::wstring& name) {
		auto value = prop.GetData<XMFLOAT4>();
		ss << name << "\n" << value->x << " " << value->y << " " << value->z << " " << value->w << "\n";
		});

	typeToWriteFuncMap.emplace(typeid(int), [&](Property& prop, std::wstring& name) {
		ss << name << "\n" << *prop.GetData<int>() << "\n";
		});

	typeToWriteFuncMap.emplace(typeid(std::string), [&](Property& prop, std::wstring& name) {
		auto str = prop.GetData<std::string>();
		ss << name << "\n" << str->c_str() << "\n";
		});

	typeToWriteFuncMap.emplace(typeid(std::wstring), [&](Property& prop, std::wstring& name) {
		auto wstr = prop.GetData<std::wstring>();
		ss << name << "\n" << wstr->data() << "\n";
		});

	typeToWriteFuncMap.emplace(typeid(TextureData), [&](Property& prop, std::wstring& name) {
		auto textureData = prop.GetData<TextureData>();
		ss << name << "\n" << textureData->filename.c_str() << "\n";
		});

	typeToWriteFuncMap.emplace(typeid(MeshComponentData), [&](Property& prop, std::wstring& name) {
		auto meshComponentData = prop.GetData<MeshComponentData>();
		ss << name << "\n" << meshComponentData->filename.c_str() << "\n";
		});

	typeToWriteFuncMap.emplace(typeid(UID), [&](Property& prop, std::wstring& name) {
		auto uid = prop.GetData<UID>();
		ss << name << "\n";
		ss << *uid << "\n";
		});

	typeToWriteFuncMap.emplace(typeid(VEnum), [&](Property& prop, std::wstring& name) {
		auto vEnum = prop.GetData<VEnum>();
		ss << name << "\n";
		ss << vEnum->GetValue().c_str() << "\n";
		});
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

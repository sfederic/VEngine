#include "Serialiser.h"
#include <DirectXMath.h>
#include <typeindex>
#include "Properties.h"
#include "Render/PipelineObjects.h"
#include "Actors/Actor.h"
#include "World.h"
#include "Render/RenderTypes.h"
#include "VString.h"

using namespace DirectX;

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
}

Serialiser::~Serialiser()
{
	//A bit counter-intuitive to put file opening for the stringstream in destructor,
	//but otherwise the entire file is overwritten from the start, meaning you miss out 
	//on the safety stringstream is lending as a temp buffer during crashes.
	ofs.open(filename.c_str(), (std::ios_base::openmode)mode);
	if (ofs.fail())
	{
		throw;
	}

	ofs << ss.str();

	ofs.flush();
	ofs.close();
}

void Serialiser::Serialise(Properties& props)
{
	for (auto& prop : props.propMap)
	{
		const std::wstring wname = VString::stows(prop.first);
		const std::string name = prop.first;

		if (props.CheckType<float>(name))
		{
			ss << wname << "\n" << *props.GetData<float>(name) << "\n";
		}
		else if (props.CheckType<XMFLOAT2>(name))
		{
			XMFLOAT2* value = props.GetData<XMFLOAT2>(name);
			ss << wname << "\n" << value->x << " " << value->y << "\n";
		}		
		else if (props.CheckType<XMFLOAT3>(name))
		{
			XMFLOAT3* value = props.GetData<XMFLOAT3>(name);
			ss << wname << "\n" << value->x << " " << value->y << " " << value->z << "\n";
		}
		else if (props.CheckType<XMFLOAT4>(name))
		{
			XMFLOAT4* value = props.GetData<XMFLOAT4>(name);
			ss << wname << "\n" << value->x << " " << value->y << " " << value->z << " " << value->w << "\n";
		}
		else if (props.CheckType<bool>(name))
		{
			ss << wname << "\n" << *props.GetData<bool>(name) << "\n";
		}
		else if (props.CheckType<int>(name))
		{
			ss << wname << "\n" << *props.GetData<int>(name) << "\n";
		}
		else if (props.CheckType<std::string>(name))
		{
			std::string* str = props.GetData<std::string>(name);
			ss << wname << "\n" << str->c_str() << "\n";
		}		
		else if (props.CheckType<std::wstring>(name))
		{
			std::wstring* wstr = props.GetData<std::wstring>(name);
			ss << wname << "\n" << VString::wstos(wstr->data()).c_str() << "\n";
		}	
		else if (props.CheckType<TextureData>(name))
		{
			TextureData* textureData = props.GetData<TextureData>(name);
			ss << wname << "\n" << textureData->filename.c_str() << "\n";
		}
		else if (props.CheckType<ShaderData>(name))
		{
			ShaderData* shaderData = props.GetData<ShaderData>(name);
			ss << wname << "\n" << shaderData->filename.c_str() << "\n";
		}		
		else if (props.CheckType<MeshComponentData>(name))
		{
			MeshComponentData* meshComponentData = props.GetData<MeshComponentData>(name);
			ss << wname << "\n" << meshComponentData->filename.c_str() << "\n";
		}
		else if (props.CheckType<std::vector<Actor*>>(name))
		{
			auto actors = props.GetData<std::vector<Actor*>>(name);
			ss << wname << "\n";
			ss << actors->size() << "\n";
			for (auto actor : *actors)
			{
				ss << VString::stows(actor->name) << "\n";
			}
		}
		else if (props.CheckType<Transform>(name))
		{
			Transform* transform = props.GetData<Transform>(name);
			ss << wname << "\n";
			ss << transform->position.x << " " << transform->position.y << " " << transform->position.z << " ";
			ss << transform->scale.x << " " << transform->scale.y << " " << transform->scale.z << " ";
			ss << transform->rotation.x << " " << transform->rotation.y << " " << transform->rotation.z << " " << transform->rotation.w << "\n";
		}
		else if (props.CheckType<UID>(name))
		{
			UID* uid = props.GetData<UID>(name);
			ss << wname << "\n";
			ss << *uid << "\n";
		}
		else if (props.CheckType<Actor*>(name))
		{
			Actor** actor = props.GetData<Actor*>(name);
			if (actor[0])
			{
				ss << wname << "\n";
				ss << VString::stows(actor[0]->name) << "\n";
			}
		}
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
		else if (props.CheckType<std::vector<Actor*>>(name))
		{
			wchar_t actorName[512]{};
			int actorsSize = 0;

			is >> actorsSize;

			//progress to next line (empty value)
			is.getline(actorName, 512);

			auto actors = props.GetData<std::vector<Actor*>>(name);
			actors->reserve(actorsSize);

			for (int i = 0; i < actorsSize; i++)
			{
				is.getline(actorName, 512);
				std::wstring actorNameStr(actorName);
				Actor* foundActor = world.GetActorByName(VString::wstos(actorNameStr));
				actors->push_back(foundActor);
			}
		}
		else if (props.CheckType<Transform>(name))
		{
			Transform* transform = props.GetData<Transform>(name);

			is >> transform->position.x;
			is >> transform->position.y;
			is >> transform->position.z;

			is >> transform->scale.x;
			is >> transform->scale.y;
			is >> transform->scale.z;

			is >> transform->rotation.x;
			is >> transform->rotation.y;
			is >> transform->rotation.z;
			is >> transform->rotation.w;
		}
		else if (props.CheckType<UID>(name))
		{
			UID* uid = props.GetData<UID>(name);
			is >> *uid;
		}
		else if (props.CheckType<Actor*>(name))
		{
			Actor** actor = props.GetData<Actor*>(name);
			std::wstring actorName;
			is >> actorName;
			Actor* foundActor = world.GetActorByName(VString::wstos(actorName));
			actor[0] = foundActor;
		}
	}
}

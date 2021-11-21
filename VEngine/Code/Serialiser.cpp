#include "Serialiser.h"
#include <DirectXMath.h>
#include <typeindex>
#include "Properties.h"
#include "Render/PipelineObjects.h"
#include "Actors/Actor.h"
#include "World.h"
#include "Render/RenderTypes.h"

using namespace DirectX;

Serialiser::Serialiser(const std::string filename, const OpenMode mode)
{
	ofs.open(filename.c_str(), (std::ios_base::openmode)mode);
	if (ofs.fail())
	{
		throw;
	}
}

Serialiser::~Serialiser()
{
	ofs << ss.str();

	ofs.flush();
	ofs.close();
}

void Serialiser::Serialise(Properties props)
{
	for (auto& prop : props.propMap)
	{
		const std::string& name = prop.first;

		if (props.CheckType<float>(name))
		{
			ss << name << "\n" << *props.GetData<float>(name) << "\n";
		}
		else if (props.CheckType<XMFLOAT2>(name))
		{
			XMFLOAT2* value = props.GetData<XMFLOAT2>(name);
			ss << name << "\n" << value->x << " " << value->y << "\n";
		}		
		else if (props.CheckType<XMFLOAT3>(name))
		{
			XMFLOAT3* value = props.GetData<XMFLOAT3>(name);
			ss << name << "\n" << value->x << " " << value->y << " " << value->z << "\n";
		}
		else if (props.CheckType<XMFLOAT4>(name))
		{
			XMFLOAT4* value = props.GetData<XMFLOAT4>(name);
			ss << name << "\n" << value->x << " " << value->y << " " << value->z << " " << value->w << "\n";
		}
		else if (props.CheckType<bool>(name))
		{
			ss << name << "\n" << *props.GetData<bool>(name) << "\n";
		}
		else if (props.CheckType<int>(name))
		{
			ss << name << "\n" << *props.GetData<int>(name) << "\n";
		}
		else if (props.CheckType<std::string>(name))
		{
			std::string* str = props.GetData<std::string>(name);
			ss << name << "\n" << str->c_str() << "\n";
		}	
		else if (props.CheckType<TextureData>(name))
		{
			TextureData* textureData = props.GetData<TextureData>(name);
			ss << name << "\n" << textureData->filename << "\n";
		}
		else if (props.CheckType<std::vector<Actor*>>(name))
		{
			auto actors = props.GetData<std::vector<Actor*>>(name);
			ss << name << "\n";
			ss << actors->size() << "\n";
			for (auto actor : *actors)
			{
				ss << actor->name << "\n";
			}
		}
		else if (props.CheckType<Transform>(name))
		{
			Transform* transform = props.GetData<Transform>(name);
			ss << name << "\n";
			ss << transform->position.x << " " << transform->position.y << " " << transform->position.z << " ";
			ss << transform->scale.x << " " << transform->scale.y << " " << transform->scale.z << " ";
			ss << transform->rotation.x << " " << transform->rotation.y << " " << transform->rotation.z << " " << transform->rotation.w << "\n";
		}
		else if (props.CheckType<UID>(name))
		{
			UID* uid = props.GetData<UID>(name);
			ss << name << "\n";
			ss << *uid << "\n";
		}
		else if (props.CheckType<Actor*>(name))
		{
			Actor** actor = props.GetData<Actor*>(name);
			if (actor[0])
			{
				ss << name << "\n";
				ss << actor[0]->name << "\n";
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
}

Deserialiser::~Deserialiser()
{
	is.close();
}

void Deserialiser::Deserialise(Properties props)
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
			char propString[512];
			is.getline(propString, 512);
			std::string* str = props.GetData<std::string>(name);
			str->assign(propString);
		}
		else if (props.CheckType<TextureData>(name))
		{
			char propString[512];
			is.getline(propString, 512);
			TextureData* textureData = props.GetData<TextureData>(name);
			textureData->filename.assign(propString);
		}
		else if (props.CheckType<std::vector<Actor*>>(name))
		{
			char actorName[512];
			int actorsSize = 0;

			is >> actorsSize;

			//progress to next line (empty value)
			is.getline(actorName, 512);

			auto actors = props.GetData<std::vector<Actor*>>(name);
			actors->reserve(actorsSize);

			for (int i = 0; i < actorsSize; i++)
			{
				is.getline(actorName, 512);
				std::string actorNameStr(actorName);
				Actor* foundActor = world.GetActorByName(actorNameStr);
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
			std::string actorName;
			is >> actorName;
			Actor* foundActor = world.GetActorByName(actorName);
			actor[0] = foundActor;
		}
	}
}

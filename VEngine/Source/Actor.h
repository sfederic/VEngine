#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <vector>
#include <memory>
#include <string>
#include "RenderTypes.h"
#include <typeindex>
#include "FBXImporter.h"
#include "AnimationStructures.h"
#include "Transform.h"
#include "RenderSystem.h"
#include <typeindex>
#include <map>

using namespace DirectX;

class Buffer;
class Sampler;
class RasterizerState;
class Texture;
class ShaderResourceView;
class BlendState;
class RenderSystem;

struct Properties
{
	template <typename T>
	void Register(const char* name, T* value)
	{
		propertyMap[typeid(*value)] = std::make_pair(name, (void*)&value);
	}

	std::unordered_map<std::type_index, std::pair<const char*, void*>> propertyMap;
};

class Actor
{
public:
	Actor();
	virtual void Tick(float deltaTime)
	{
		
	}

	//Returns all the hand-defined properties in this function
	virtual Properties GetProperties() 
	{
		Properties properties;
		return properties;
	}

	template <class ActorType>
	bool IsA()
	{
		if (typeid(this) == typeid(ActorType))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	XMVECTOR GetPositionVector();
	XMFLOAT3 GetPositionFloat3();
	void SetPosition(XMVECTOR v);
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 pos);

	void SetRotation(XMVECTOR quaternion);
	void SetRotation(XMVECTOR axis, float angle);
	void SetRotation(float roll, float pitch, float yaw);
	void SetRotation(XMFLOAT3 euler);
	XMFLOAT4 GetRotationQuat();

	XMMATRIX GetTransformationMatrix();
	void SetTransformationMatrix(XMMATRIX m);

	XMFLOAT3 GetPitchYawRoll();

	XMFLOAT3 GetScale();
	void AddScale(float scale);
	void AddScale(float x, float y, float z);
	void AddScale(XMFLOAT3 scale);
	void SetScale(float x, float y, float z);
	void SetScale(XMVECTOR scale);
	void SetScale(XMFLOAT3 scale);

	XMVECTOR GetForwardVector();
	XMVECTOR GetRightVector();
	XMVECTOR GetUpVector();

	void Move(float d, XMVECTOR direction);
	ActorSystem* GetActorSystem();

	Transform transform;
	Material material;

	double currentAnimationTime = 0.0;

	int vertexBufferOffset;
	bool bRender = true;
	bool bPicked = false;

	wchar_t name[64];

	ActorSystem* linkedActorSystem;
};

struct PipelineView
{
	void Create();

	Buffer* vertexBuffer;
	Buffer* indexBuffer;
	Buffer* instanceBuffer;
	Sampler* samplerState;
	RasterizerState* rastState;
	Texture* texture;
	ShaderResourceView* srv;
	BlendState* blendState;
};

class ActorSystem
{
public:
	ActorSystem()
	{
		pso.Create();
	}

	virtual void Tick(float deltaTime) = 0;

	//These two functions are to get around classes calling CreateActors<>() with their own actor types.
	virtual void SpawnActors(int numToSpawn) = 0;
	virtual void SpawnActor(Transform transform) = 0;

	template <class ActorType>
	void SetActorSize()
	{
		sizeofActor = sizeof(ActorType);
	}

	template <class ActorType>
	ActorType* AddActor(Transform transform)
	{
		ActorType* actor = new ActorType();
		actor->transform = transform;
		actor->vertexBufferOffset = (int)(actors.size() * modelData.GetByteWidth());
		wcsncpy(actor->name, name.c_str(), name.size());
		actor->material = this->material;
		actor->linkedActorSystem = this;

		actors.push_back(actor);
		return actor;
	}

	template <class ActorType>
	void Init(int numActorsToSpawn)
	{
		std::string filename = "Models/";
		filename += modelName;

		if (FBXImporter::Import(filename.c_str(), modelData, this))
		{
			//Spawn actors, setup components
			actors.reserve(numActorsToSpawn);
			for (int i = 0; i < numActorsToSpawn; i++)
			{
				AddActor<ActorType>(Transform());
			}

			//Setup buffers
			UINT byteWidth = modelData.GetByteWidth();
			numVertices = (byteWidth * actors.size()) / sizeof(Vertex);

			gRenderSystem.CreateVertexBuffer(byteWidth, modelData.verts.data(), this);

			if (bInstancingActors)
			{
				std::vector<XMMATRIX> actorModelMatrices;
				actorModelMatrices.reserve(numActorsToDrawOnInstance);
				for (int i = 0; i < actors.size(); i++)
				{
					if (i < actors.size())
					{
						actorModelMatrices.push_back(actors[i]->GetTransformationMatrix());
					}
					else
					{
						actorModelMatrices.push_back(XMMatrixIdentity());
					}
				}

				gRenderSystem.CreateConstantInstanceBuffer(sizeof(InstanceData) * actors.size(), actorModelMatrices.data(), this);
			}

			//TODO: index buffers
			//UINT indicesByteWidth = modelData.indices.size() * sizeof(uint16_t);
			//indexBuffer = renderSystem->CreateDefaultBuffer(indicesByteWidth, D3D11_BIND_INDEX_BUFFER, modelData.indices.data());

			//Sampler, texture setup
			gRenderSystem.CreateSamplerState(GetSamplerState());
			gRenderSystem.CreateTexture(this);

			size_t stride = sizeof(Vertex);

			//Bounding setup
			BoundingBox::CreateFromPoints(boundingBox, modelData.verts.size(), &modelData.verts[0].pos, stride);
			BoundingSphere::CreateFromPoints(boundingSphere, modelData.verts.size(), &modelData.verts[0].pos, stride);

			bHasBeenInitialised = true;
		}
		else
		{
			DebugPrint("Actors failed to load");
			bHasBeenInitialised = false;
		}
	}

	void RemoveActor(int index);
	Actor* ActorSystem::GetActor(unsigned int index);

	//PSO functions
	Buffer* GetVertexBuffer();
	Buffer* GetInstanceBuffer();
	Sampler* GetSamplerState();
	RasterizerState* GetRasterizerState();
	ShaderResourceView* GetShaderView();
	Texture* GetTexture();

	void SetVertexBuffer(Buffer* vertexBuffer);
	void SetInstanceBuffer(Buffer* instanceBuffer);
	void SetSamplerState(Sampler* sampler);
	void SetRasterizerState(RasterizerState* rasterizerState);
	void SetShaderView(ShaderResourceView* shaderView);
	void SetTexture(Texture* texture);

	void RecreateTexture();
	void RecreateShader();
	void RecreateModel();

	void Cleanup();
	void ResetActorNames();

	template <class ActorType>
	bool IsA()
	{
		if (typeid(this) == typeid(ActorType))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	ModelData modelData;
	
	//SkinnedData skinnedData;
	Animation animData;

	Material material;

	PipelineView pso;

	BoundingBox boundingBox;
	BoundingSphere boundingSphere;

	size_t numVertices;

	std::vector<Actor*> actors;

	std::wstring shaderName;
	std::wstring textureName;
	std::wstring name;
	std::string modelName;

	//Size of the actor system's linked actor
	uint32_t sizeofActor = 0;

	bool bAnimated = false; //Whether model has any animation data. Is set in FBX import.
	bool bHasSkeletalAnimation;
	bool bRender = true;
	bool bHasBeenInitialised = false;

	bool bInstancingActors = false; //bool for setting system to use instancing in renderer
	uint32_t numActorsToDrawOnInstance;
};
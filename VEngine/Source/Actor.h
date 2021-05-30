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
#include <unordered_map>
#include "Serialise.h"
#include "Components/Component.h"
#include "PipelineView.h"

using namespace DirectX;

class RenderSystem;
class Component;

class Actor
{
public:
	Actor();
	virtual void Tick(float deltaTime);

	virtual Properties GetProperties();

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
	XMFLOAT3 GetPitchYawRoll();
	XMFLOAT3 GetScale();
	void SetScale(float x, float y, float z);
	void SetScale(XMVECTOR scale);
	void SetScale(XMFLOAT3 scale);
	XMVECTOR GetForwardVector();
	XMVECTOR GetRightVector();
	XMVECTOR GetUpVector();
	void Move(float d, XMVECTOR direction);
	ActorSystem* GetActorSystem();
	void Destroy();

	template <class T>
	void AddComponent(T* component)
	{
		component = new T();
	}

	Transform transform;
	Material material;
	std::string name;
	ActorSystem* linkedActorSystem;
	double currentAnimationTime = 0.0;
	bool bRender = true;
};

class ActorSystem
{
public:
	ActorSystem();
	void Serialise(std::ostream& os);
	void SerialiseAsTemplate(std::ostream& os);
	void Deserialise(std::istream& is);
	void DeserialiseAsTemplate(std::istream& is);
	virtual void Tick(float deltaTime);
	virtual void SpawnActors(int numToSpawn);
	virtual Actor* SpawnActor(Transform transform);

	template <class ActorType>
	void SetActorSize()
	{
		sizeofActor = sizeof(ActorType);
	}

	template <class ActorType>
	Actor* AddActor(Transform transform)
	{
		ActorType* actor = new ActorType();
		actor->transform = transform;
		actor->material = this->material;
		actor->linkedActorSystem = this;

		actors.push_back(actor);
	
		actor->name = this->name + std::to_string(actors.size() - 1);

		if (bHasBeenInitialised)
		{
			//Structured buffer needs to be rebuilt
			//TODO: Maybe look into doing some std::vector-tier allocation where reallocation on this buffer
			//happens when it passes a threshold (eg. actors.size() > 64)
			CreateStructuredBuffer();
		}

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

			CreateStructuredBuffer();

			//Sampler, texture setup
			gRenderSystem.CreateSamplerState(GetSamplerState());
			gRenderSystem.CreateTexture(this);

			size_t stride = sizeof(Vertex);

			//Bounding setup
			BoundingOrientedBox::CreateFromPoints(boundingBox, modelData.verts.size(), &modelData.verts[0].pos, stride);
			BoundingSphere::CreateFromPoints(boundingSphere, modelData.verts.size(), &modelData.verts[0].pos, stride);

			bHasBeenInitialised = true;
		}
		else
		{
			DebugPrint("Actors failed to load");
			bHasBeenInitialised = false;
		}
	}

	void RemoveActor(Actor* actor);
	void RemoveActor(int index);
	Actor* GetActor(unsigned int index);

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

	void CreateStructuredBuffer();

	void RecreateTexture();
	void RecreateShader();
	void RecreateModel();

	void Cleanup();

	ID3D11Buffer* instancedDataStructuredBuffer;
	ID3D11ShaderResourceView* instancedDataSrv;

	ModelData modelData;
	Animation animData;
	Material material;
	PipelineView pso;

	//Everything's a BoundingOrientedBox, but maybe just a BoundingBox is faster in some places.
	//REF:https://www.gamasutra.com/view/feature/131833/when_two_hearts_collide_.php
	BoundingOrientedBox boundingBox;
	BoundingSphere boundingSphere;

	std::vector<Actor*> actors;

	std::string shaderName;
	std::string textureName;
	std::string name;
	std::string modelName;

	uint32_t sizeofActor = 0;
	size_t numVertices;

	bool bAnimated = false;
	bool bRender = true;
	bool bHasBeenInitialised = false;
};

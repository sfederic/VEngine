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
#include "Component.h"
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
	int vertexBufferOffset;
	bool bRender = true;
};

class ActorSystem
{
public:
	ActorSystem();
	void Serialise(std::ostream& os);
	void Deserialise(std::istream& is);
	virtual void Tick(float deltaTime) = 0;
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

			std::vector<XMMATRIX> actorModelMatrices;
			actorModelMatrices.reserve(actors.size());
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

			//Setup structured buffer
			instancedDataStructuredBuffer = gRenderSystem.CreateStructuredBuffer(sizeof(InstanceData) * actors.size(), sizeof(InstanceData), actorModelMatrices.data());
			D3D11_SHADER_RESOURCE_VIEW_DESC sbDesc = {};
			sbDesc.Format = DXGI_FORMAT_UNKNOWN;
			sbDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
			sbDesc.BufferEx.NumElements = actors.size();
			HR(gRenderSystem.device->CreateShaderResourceView(instancedDataStructuredBuffer, &sbDesc, &instancedDataSrv));

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

	std::wstring shaderName;
	std::wstring textureName;
	std::string name;
	std::string modelName;

	uint32_t sizeofActor = 0;
	size_t numVertices;

	bool bAnimated = false;
	bool bRender = true;
	bool bHasBeenInitialised = false;
};

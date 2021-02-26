#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <vector>
#include <memory>
#include <string>
#include "RenderTypes.h"
#include <typeindex>
#include "FBXImporter.h"

using namespace DirectX;

class IBuffer;
class ISampler;
class IRasterizerState;
class ITexture;
class IShaderView;

struct ID3D12PipelineState;

enum class EActorSystemID
{

};

class Actor
{
public:
	Actor();
	virtual void Tick(float deltaTime)
	{

	}

	XMVECTOR GetPositionVector();
	XMFLOAT3 GetPositionFloat3();
	void SetPosition(XMVECTOR v);
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 pos);
	void SetRotation(XMVECTOR axis, float angle);
	void SetRotation(float roll, float pitch, float yaw);
	void SetRotation(XMFLOAT3 rollPitchYaw);
	XMMATRIX GetRotation();
	XMFLOAT3 GetRollPitchYaw();
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

	XMMATRIX transform = XMMatrixIdentity();
	Material material;

	int vertexBufferOffset;
	bool bRender = true;
	bool bPicked = false;

	std::wstring name;
};

//TODO: move this into its own file after testing
struct PipelineView
{
	void Create();

	IBuffer* vertexBuffer;
	IBuffer* indexBuffer;
	ISampler* samplerState;
	IRasterizerState* rastState;
	ITexture* texture;
	IShaderView* srv;

	//TODO: this is no good here abstracted, but good enough now for testing
	ID3D12PipelineState* pipelineState;
};

class ActorSystem
{
public:
	ActorSystem() {}
	virtual void Tick(float deltaTime) {}

	template <class ActorType>
	void CreateActors(class IRenderSystem* renderSystem, int numActorsToSpawn)
	{
		char filename[128] = {};
		strcat_s(filename, "Models/");
		strcat_s(filename, modelName);

		//if (LoadOBJFile(filename, modelData))
		if (FBXImporter::Import(filename, modelData))
		{
			UINT byteWidth = modelData.GetByteWidth();
			numVertices = (byteWidth * actors.size()) / sizeof(Vertex);
			renderSystem->CreateVertexBuffer(byteWidth, modelData.verts.data(), this);
			UINT indicesByteWidth = modelData.indices.size() * sizeof(uint16_t);
			//indexBuffer = renderSystem->CreateDefaultBuffer(indicesByteWidth, D3D11_BIND_INDEX_BUFFER, modelData.indices.data());

			renderSystem->CreateSamplerState(this);
			renderSystem->CreateTexture(this);

			size_t stride = sizeof(Vertex);

			BoundingBox::CreateFromPoints(boundingBox, modelData.verts.size(), &modelData.verts[0].pos, stride);
			BoundingSphere::CreateFromPoints(boundingSphere, modelData.verts.size(), &modelData.verts[0].pos, stride);

			actors.reserve(numActorsToSpawn);
			for (int i = 0; i < numActorsToSpawn; i++)
			{
				//TODO: I'm gonna need the smart pointers here to deal with the eventual Tick() virtual calls
				ActorType* actor = new ActorType();
				actor->transform.r[3] = XMVectorSet(i, i, i, 1.f);
				actor->vertexBufferOffset = i * modelData.GetByteWidth();
				actor->name = name;
				std::wstring indexString = std::to_wstring(i);
				actor->name += indexString;

				actors.push_back(actor);
			}
		}
		else
		{
			DebugPrint("Actors failed to load");
		}
	}

	template <class ActorType>
	Actor* AddActor(XMVECTOR spawnPosition)
	{
		ActorType* actor = new ActorType();
		actor.SetPosition(spawnPosition);
		actor.vertexBufferOffset = (int)(actors.size() * modelData.GetByteWidth());

		actors.push_back(actor);
		return actor;
	}

	void RemoveActor(int index);
	Actor* GetActor(unsigned int index);
	
	//PSO functions
	void* GetVertexBuffer();
	void* GetSamplerState();
	void* GetRasterizerState();
	void* GetShaderView();
	void* GetTexture();

	void SetVertexBuffer(IBuffer* vertexBuffer);
	void SetSamplerState(ISampler* sampler);
	void SetRasterizerState(IRasterizerState* rasterizerState);
	void SetShaderView(IShaderView* shaderView);
	void SetTexture(ITexture* texture);

	template <class SystemType>
	bool IsA()
	{
		if (typeid(this) == typeid(SystemType))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	ModelData modelData;
	
	PipelineView pso;

	BoundingBox boundingBox;
	BoundingSphere boundingSphere;

	size_t numVertices;

	std::vector<Actor*> actors;

	std::wstring name;

	EActorSystemID id;

	const wchar_t* shaderName;
	const wchar_t* textureName;
	const char* modelName;

	bool bInstancingActors; //bool for setting system to use instancing
};
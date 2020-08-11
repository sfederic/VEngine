#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <vector>
#include <memory>
#include "RenderSystem.h" //TODO: Get rid of this and move struct Vertex into seperate file

using namespace DirectX;

//TODO: fix this up
enum class PickedAxis
{
	None,
	X,
	Y,
	Z
};

enum class EActorSystemID
{
	Actor,
	DebugSphere,
	DebugBox
};

struct ModelData
{
	UINT GetByteWidth()
	{
		return (UINT)(sizeof(Vertex) * verts.size());
	}

	std::vector<Vertex> verts;
	std::vector<uint16_t> indices;
};

//Mario 64 used the term Actor
class Actor
{
public:
	Actor();
	//virtual void Tick(float deltaTime) = 0;
	XMVECTOR GetPositionVector();
	XMFLOAT3 GetPositionFloat3();
	void SetPosition(XMVECTOR v);
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 pos);
	void SetRotation(XMVECTOR axis, float angle);
	XMMATRIX GetRotation();
	XMFLOAT3 GetScale();
	void SetScale(float x, float y, float z);
	void SetScale(XMVECTOR scale);
	void SetScale(XMFLOAT3 scale);
	XMVECTOR GetForwardVector();
	XMVECTOR GetRightVector();
	XMVECTOR GetUpVector();
	void Move(float d, XMVECTOR direction);

	XMMATRIX transform = XMMatrixIdentity();

	int vertexBufferOffset;
	bool bRender = true;
	bool bPicked = false;

	//TODO: Temp. replace with new actor 
	PickedAxis pickedAxis;
};

//TODO: I want to keep the batched state change rendering I have now.
//Create a few new bools and sort the World list of ActorSystems based on those bools, then render
class ActorSystem
{
public:
	ActorSystem() {}
	//virtual void Tick(float deltaTime) = 0;
	void CreateActors(class RenderSystem* dx, int numActorsToSpawn);
	void AddActor(XMVECTOR spawnPosition);
	void RemoveActor(int index);
	Actor* GetActor(unsigned int index);

	ModelData modelData;

	struct ID3D11Buffer* vertexBuffer;
	struct ID3D11Buffer* indexBuffer;
	struct ID3D11SamplerState* samplerState;
	struct ID3D11RasterizerState* rastState;

	struct ID3D11Resource* texture;
	struct ID3D11ShaderResourceView* srv;

	BoundingBox boundingBox;
	BoundingSphere boundingSphere;

	UINT64 numVertices; //Frustrum culling is going to make a mess of this 

	std::vector<Actor> actors;

	const wchar_t* shaderName = L"shaders.hlsl";
	const wchar_t* textureName = L"texture.png";
	const char* modelName = "cube.obj";

	EActorSystemID id = EActorSystemID::Actor;

	bool bInstancingActors; //bool for setting system to use instancing
};
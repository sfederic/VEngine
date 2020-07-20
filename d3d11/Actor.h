#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <vector>
#include "Obj.h"

using namespace DirectX;

class Actor
{
public:
	Actor();
	//virtual void Tick(float deltaTime) = 0;
	XMVECTOR GetPositionVector();
	XMFLOAT3 GetPositionFloat3();
	void SetPosition(XMVECTOR v);
	void SetPosition(float x, float y, float z);
	void SetRotation(XMVECTOR axis, float angle);
	XMMATRIX GetRotation();
	XMFLOAT3 GetScale();
	void SetScale(float x, float y, float z);
	void SetScale(XMVECTOR scale);

	//BoundingBox boundingBox;
	//BoundingSphere boundingSphere;

	XMMATRIX transform = XMMatrixIdentity();

	int vertexBufferOffset;
	bool bRender = true;
};

class ActorSystem
{
public:
	ActorSystem() {}
	//virtual void Tick(float deltaTime) = 0;
	void CreateActors(const char* modelFilename, class RenderSystem* dx, int numActorsToSpawn);
	void AddActor();
	void RemoveActor(int index);

	OBJData modelData;

	struct ID3D11Buffer* vertexBuffer;
	struct ID3D11Buffer* indexBuffer;

	//Wonder if I can make it so that only the system needs 1 copy of each and the uniform scaling does the rest
	BoundingBox boundingBox;
	BoundingSphere boundingSphere;

	UINT64 numVertices; //Frustrum culling is going to make a mess of this 

	std::vector<Actor> actors;

	const wchar_t* shaderName = L"shaders.hlsl";

	bool bInstancingActors; //bool for setting system to use instancing
};
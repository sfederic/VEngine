#pragma once

#include <d3d11.h> //TODO: fix up forward declarations
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <vector>
#include "Obj.h"

using namespace DirectX;

//TODO: split up actor and actorsystem into files

//Not liking the per actor basis for construction at the moment. Need to find a balance for batch rendering and ease of use
class Actor
{
public:
	Actor();
	void tick() {};
	XMVECTOR GetActorPosition();
	void SetActorPosition(XMVECTOR v);
	void SetActorPosition(float x, float y, float z);
	void SetActorRotation(XMVECTOR axis, float angle);
	XMMATRIX GetActorRotation(); //Don't like this

	//TODO: do actors need names later for the editor?

	int vertexBufferOffset;
	XMMATRIX transform = XMMatrixIdentity();
	
};

class ActorSystem
{
public:
	void CreateActors(const char* modelFilename, class DXUtil* dx, int numActorsToSpawn);

	OBJData modelData;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	//Wonder if I can make it so that only the system needs 1 copy of each and the uniform scaling does the rest
	BoundingBox boundingBox;
	BoundingSphere boundingSphere;

	UINT64 numVertices; //Frustrum culling is going to make a mess of this 

	std::vector<Actor> actors;

	bool bInstancingActors; //bool for setting system to use instancing
};
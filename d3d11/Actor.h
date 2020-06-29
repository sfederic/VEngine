#pragma once

#include <d3d11.h> //TODO: fix up forward declarations
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Obj.h"

using namespace DirectX;

//Not liking the per actor basis for construction at the moment. Need to find a balance for batch rendering and ease of use
class Actor
{
public:
	Actor();
	void CreateActor(const char* modelFilename, class DXUtil* dx);
	void tick() {};

	OBJData modelData;

	BoundingBox boundingBox;
	BoundingSphere boundingSphere;

	XMMATRIX transform;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};


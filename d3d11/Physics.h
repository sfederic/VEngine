#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct Ray
{
	XMVECTOR origin;
	XMVECTOR direction;
};

namespace Physics
{
	void DrawRayDebug(XMVECTOR rayOrigin, XMVECTOR rayDir, float distance, ID3D11Buffer* debugBuffer, DXUtil* dx);
	void Raycast(Ray& ray, int sx, int sy, Camera* camera, XMMATRIX& worldMatrix);
};


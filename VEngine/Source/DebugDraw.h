#pragma once

#include <DirectXMath.h>
#include "Transform.h"
#include "RenderTypes.h"

using namespace DirectX;

namespace DebugDraw
{
	void DrawSphere(XMFLOAT3 pos, float scale = 1.0f);
	void DrawBox(Transform transform);
	void DrawRay(XMVECTOR rayOrigin, XMVECTOR rayDir, float distance);
};

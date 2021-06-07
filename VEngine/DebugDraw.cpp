#include "DebugDraw.h"
#include "DebugSphere.h"

void DebugDraw::DrawSphere(XMFLOAT3 pos, float scale)
{
	Transform transform;
	transform.position = pos;
	transform.scale = XMFLOAT3(scale, scale, scale);

	debugSphere.debugSphereTransforms.push_back(transform);
}

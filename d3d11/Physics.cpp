#include "Physics.h"
#include "RenderSystem.h"
#include "Camera.h"
#include "UISystem.h"
#include "Actor.h"
#include "DebugMenu.h"
#include "CoreSystem.h"
#include "Debug.h"

void DrawRayDebug(XMVECTOR rayOrigin, XMVECTOR rayDir, float distance, ID3D11Buffer* debugBuffer, RenderSystem* dx)
{
	Vertex v1 = {}, v2 = {};
	XMStoreFloat3(&v1.pos, rayOrigin);
	if (distance <= 0.f)
	{
		distance = 10000.f;
	}
	XMVECTOR dist = rayDir * distance;
	XMVECTOR rayEnd = rayOrigin + dist;
	XMStoreFloat3(&v2.pos, rayEnd);

	dx->debugLines.push_back(v1);
	dx->debugLines.push_back(v2);

	dx->context->UpdateSubresource(debugBuffer, 0, nullptr, dx->debugLines.data(), 0, 0);
}

bool Raycast(Ray& ray, int sx, int sy, Camera* camera, ActorSystem* actorSystem)
{
	float vx = (2.f * sx / coreSystem.windowWidth - 1.0f) / camera->proj.r[0].m128_f32[0];
	float vy = (-2.f * sy / coreSystem.windowHeight + 1.0f) / camera->proj.r[1].m128_f32[1];

	ray.origin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	ray.direction = XMVectorSet(vx, vy, 1.f, 0.f);

	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(camera->view), camera->view);
	//Big problem with the model matrix. Before I was wrongly using the model per actor and re-calculating. Hoisting Identity out fixed that.
	XMMATRIX invModel = XMMatrixInverse(&XMMatrixDeterminant(XMMatrixIdentity()), XMMatrixIdentity());
	XMMATRIX toLocal = XMMatrixMultiply(invView, invModel);

	ray.origin = XMVector3TransformCoord(ray.origin, toLocal);
	ray.direction = XMVector3TransformNormal(ray.direction, toLocal);
	ray.direction = XMVector3Normalize(ray.direction);

	for (int i = 0; i < actorSystem->actors.size(); i++)
	{
		actorSystem->boundingBox.Center = actorSystem->actors[i].GetPositionFloat3();
		actorSystem->boundingBox.Extents = actorSystem->actors[i].GetScale();

		if (actorSystem->boundingBox.Intersects(ray.origin, ray.direction, ray.distance))
		{
			ray.actorIndex = i;
			DebugPrint("hit %d\n", ray.actorIndex);
			return true;
		}
	}

	return false;
}

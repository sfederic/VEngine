#include "Physics.h"
#include "DXUtil.h"
#include "Camera.h"

void DrawRayDebug(XMVECTOR rayOrigin, XMVECTOR rayDir, float distance, ID3D11Buffer* debugBuffer, DXUtil* dx)
{
	Vertex v1 = {}, v2 = {};
	XMStoreFloat3(&v1.pos, rayOrigin);
	XMVECTOR dist = rayDir * distance;
	XMVECTOR rayEnd = rayOrigin + dist;
	XMStoreFloat3(&v2.pos, rayEnd);

	dx->debugLines.push_back(v1);
	dx->debugLines.push_back(v2);

	dx->context->UpdateSubresource(debugBuffer, 0, nullptr, dx->debugLines.data(), 0, 0);
}

void Raycast(Ray& ray, int sx, int sy, Camera* camera, XMMATRIX& worldMatrix)
{
	XMMATRIX proj = camera->proj;
	float vx = (+2.0f * sx / windowWidth - 1.0f) / proj.r[0].m128_f32[0];
	float vy = (-2.0f * sy / windowHeight + 1.0f) / proj.r[1].m128_f32[1];

	ray.origin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	ray.direction = XMVectorSet(vx, vy, 1.f, 0.f);

	XMMATRIX view = camera->view;

	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX W = worldMatrix;
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);
	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	ray.origin = XMVector3TransformCoord(ray.origin, toLocal);
	ray.direction = XMVector3TransformNormal(ray.direction, toLocal);

	//This little offset here worries me. Without it, the ray is a few pixels off on the y-axis
	//ray.direction.m128_f32[1] -= 0.040f;

	ray.direction = XMVector3Normalize(ray.direction);
}
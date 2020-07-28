#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct Ray
{
	XMVECTOR origin;
	XMVECTOR direction;

	float distance;
	int actorIndex = 0;
	int actorSystemIndex = 0;
};

void DrawRayDebug(XMVECTOR rayOrigin, XMVECTOR rayDir, float distance, class ID3D11Buffer* debugBuffer);
bool Raycast(Ray& ray, XMVECTOR origin, XMVECTOR direction, class ActorSystem* actorSystem);
bool RaycastAll(Ray& ray, XMVECTOR origin, XMVECTOR direction, class World* world);
bool RaycastFromScreen(Ray& ray, int sx, int sy, class Camera* camera, class ActorSystem* actorSystem);
bool RaycastAllFromScreen(Ray& ray, int sx, int sy, Camera* camera, class World* world);

#pragma once

#include <DirectXMath.h>
#include <vector>
#include "RenderTypes.h"

using namespace DirectX;

class Actor;

struct Ray
{
	XMVECTOR origin;
	XMVECTOR direction;

	XMFLOAT3 hitPos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;

	float distance;

	int modelDataIndex; //Long as the engine is keeping off the index buffer, index will give normal and uv to pos

	int actorIndex = 0;
	int actorSystemIndex = 0;

	std::vector<Actor*> hitActors;
	Actor* hitActor;

	bool bHit = false;
};

extern std::vector<Line> debugLines;

void DrawRayDebug(XMVECTOR rayOrigin, XMVECTOR rayDir, float distance);
void ClearDebugRays();
bool Raycast(Ray& ray, XMVECTOR origin, XMVECTOR direction, class ActorSystem* actorSystem, bool fromScreen = false);
bool RaycastTriangleIntersect(Ray& ray);
bool RaycastAll(Ray& ray, XMVECTOR origin, XMVECTOR direction, class World* world);
bool RaycastFromScreen(Ray& ray, int sx, int sy, class Camera* camera, class ActorSystem* actorSystem);
bool RaycastAllFromScreen(Ray& ray);

#pragma once
#include <vector>
#include <DirectXMath.h>
#include <DirectXCollision.h>

struct IActorSystem;
struct CameraComponent;
struct Actor;

using namespace DirectX;

struct Ray
{
	XMVECTOR origin;
	XMVECTOR direction;

	XMFLOAT3 hitPos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;

	std::vector<Actor*> hitActors;
	Actor* hitActor;

	float distance = 0.f;
	int modelDataIndex; //Long as the engine is keeping off the index buffer, index will give normal and uv to pos
	int actorIndex = 0;
	int actorSystemIndex = 0;
	bool bHit = false;
};

//export void DrawRayDebug(XMVECTOR rayOrigin, XMVECTOR rayDir, float distance)
bool Raycast(Ray& ray, XMVECTOR origin, XMVECTOR direction, IActorSystem* actorSystem, bool fromScreen = false);
bool RaycastTriangleIntersect(Ray& ray);
bool RaycastAll(Ray& ray, XMVECTOR origin, XMVECTOR direction);
bool RaycastFromScreen(Ray& ray, int sx, int sy, CameraComponent* camera, IActorSystem* actorSystem);
bool RaycastAllFromScreen(Ray& ray);

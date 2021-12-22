#pragma once
#include <vector>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Physics/CollisionLayers.h"

struct IActorSystem;
struct CameraComponent;
struct Actor;

using namespace DirectX;

struct Ray
{
	XMVECTOR origin;
	XMVECTOR direction;
	
	//Position the ray has hit in world
	XMFLOAT3 hitPos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;

	//List of actors to ignore when cast
	std::vector<Actor*> actorsToIgnore;

	std::vector<Actor*> hitActors;
	Actor* hitActor = nullptr;

	//Layer to ignore on raycast
	CollisionLayers ignoreLayer = CollisionLayers::None;

	//Cut the raycast off at this point
	float range = 0.f;

	//Output distance from ray origin and hit point
	float hitDistance = 0.f;

	int actorIndex = 0;
	int actorSystemIndex = 0;
	bool bHit = false;

	Ray() {}
	Ray(Actor* actorToIgnore)
	{
		actorsToIgnore.push_back(actorToIgnore);
	}

	void AddActorsToIgnore(std::vector<Actor*>& actors);
};

//void DrawRayDebug(XMVECTOR rayOrigin, XMVECTOR rayDir, float distance);
bool Raycast(Ray& ray, XMVECTOR origin, XMVECTOR direction, float range, bool fromScreen = false);
bool RaycastTriangleIntersect(Ray& ray);
bool RaycastFromScreen(Ray& ray);

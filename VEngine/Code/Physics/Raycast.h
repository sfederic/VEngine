#pragma once

#include <vector>
#include <DirectXMath.h>
#include "Physics/CollisionLayers.h"

class IActorSystem;
struct CameraComponent;
class Actor;
struct SpatialComponent;

using namespace DirectX;

struct HitResult
{
	XMVECTOR origin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR direction = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	//Position the ray has hit in world
	XMFLOAT3 hitPos = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 hitNormal = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT2 uv = XMFLOAT2(0.f, 0.f);

	//List of actors to ignore when cast
	std::vector<Actor*> actorsToIgnore;

	std::vector<SpatialComponent*> componentsToIgnore;

	std::vector<Actor*> hitActors;

	//The closest hit actor.
	Actor* hitActor = nullptr;

	std::vector<SpatialComponent*> hitComponents;
	SpatialComponent* hitComponent = nullptr;

	//Layer to ignore on raycast
	CollisionLayers ignoreLayer = CollisionLayers::None;

	//Cut the raycast off at this point
	float range = 0.f;

	//Output distance from ray origin and hit point
	float hitDistance = 0.f;

	bool bHit = false;

	HitResult() {}
	HitResult(Actor* actorToIgnore)
	{
		actorsToIgnore.emplace_back(actorToIgnore);
	}

	template <typename T>
	T* GetHitActorAs()
	{
		auto actor = dynamic_cast<T*>(hitActor);
		if (actor)
		{
			return actor;
		}

		return nullptr;
	}

	void AddActorsToIgnore(std::vector<Actor*>& actors);
	void IgnorePlayer();

	//Result is in world space.
	XMVECTOR GetHitPosV() { return XMLoadFloat3(&hitPos); }
	XMVECTOR GetNormalV() { return XMLoadFloat3(&hitNormal); }

	std::vector<int> hitVertIndexes;
};

bool Raycast(HitResult& hitResult, XMVECTOR origin, XMVECTOR direction, float range, bool fromScreen = false);
bool Raycast(HitResult& hitResult, XMVECTOR origin, XMVECTOR end);
bool RaycastTriangleIntersect(HitResult& hitResult);
bool RaycastFromScreen(HitResult& hitResult);
void DrawDebugLine(XMVECTOR start, XMVECTOR end);

bool OrientedBoxCast(HitResult& hitResult, XMVECTOR origin, XMVECTOR end, XMFLOAT2 extents, bool drawDebug, bool clearDebugDrawWithTimer);

//Make sure the boundingbox is in world space.
bool OrientedBoxCast(HitResult& hit, BoundingOrientedBox& boundsInWorldSpace, bool drawDebug, bool clearDebugDrawWithTimer);

//Doesn't set HitResult::hitActor.
bool SimpleBoxCast(XMVECTOR center, XMFLOAT3 extents, HitResult& hitResult, bool drawDebug, bool clearDebugDrawWithTimer);

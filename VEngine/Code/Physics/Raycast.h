#pragma once

#include <vector>
#include <DirectXMath.h>
#include "Physics/CollisionLayers.h"

class IActorSystem;
struct CameraComponent;
class Actor;
struct SpatialComponent;
namespace DirectX {
	struct BoundingOrientedBox;
}

struct HitResult
{
	DirectX::XMVECTOR origin = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	DirectX::XMVECTOR direction = DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f);

	//Position the ray has hit in world
	DirectX::XMFLOAT3 hitPos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 hitNormal = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT2 uv = DirectX::XMFLOAT2(0.f, 0.f);

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

	//Whether to ignore raycast backface checks
	bool ignoreBackFaceHits = true;

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

	Actor* FindHitActor(Actor* findActor)
	{
		for (auto actor : hitActors)
		{
			if (actor == findActor)
			{
				return actor;
			}
		}

		return nullptr;
	}

	void AddActorsToIgnore(std::vector<Actor*>& actors);
	void IgnorePlayer();

	//Result is in world space.
	DirectX::XMVECTOR GetHitPosV() { return XMLoadFloat3(&hitPos); }
	DirectX::XMVECTOR GetNormalV() { return XMLoadFloat3(&hitNormal); }

	std::vector<int> hitVertIndexes;
};

bool Raycast(HitResult& hitResult, DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float range, bool fromScreen = false);
bool Raycast(HitResult& hitResult, DirectX::XMVECTOR origin, DirectX::XMVECTOR end);
bool RaycastTriangleIntersect(HitResult& hitResult);
bool RaycastFromScreen(HitResult& hitResult);
void DrawDebugLine(DirectX::XMVECTOR start, DirectX::XMVECTOR end);

bool OrientedBoxCast(HitResult& hitResult, DirectX::XMVECTOR origin, DirectX::XMVECTOR end, DirectX::XMFLOAT2 extents, bool drawDebug, bool clearDebugDrawWithTimer);

//Make sure the boundingbox is in world space.
bool OrientedBoxCast(HitResult& hit, DirectX::BoundingOrientedBox& boundsInWorldSpace, bool drawDebug, bool clearDebugDrawWithTimer);

//Doesn't set HitResult::hitActor.
bool SimpleBoxCast(DirectX::XMVECTOR center, DirectX::XMFLOAT3 extents, HitResult& hitResult, bool drawDebug, bool clearDebugDrawWithTimer);

#include <DirectXMath.h>

export module Physics.HitResult;

import Physics.CollisionLayers;
import Render.Vertex;
import <vector>;

class Actor;
class SpatialComponent;

export struct HitResult
{
	//Mainly got this here for vertex colour painting, to be able to switch brush modes and paint entire faces at once.
	std::vector<int> vertIndexesOfHitTriangleFace;
	std::vector<int> hitVertIndexes;

	//List of actors to ignore when cast
	std::vector<Actor*> actorsToIgnore;
	std::vector<SpatialComponent*> componentsToIgnore;
	std::vector<Actor*> hitActors;
	std::vector<SpatialComponent*> hitComponents;

	DirectX::XMVECTOR origin = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	DirectX::XMVECTOR direction = DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f);

	//Position the ray has hit in world
	DirectX::XMFLOAT3 hitPos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 hitNormal = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT2 uv = DirectX::XMFLOAT2(0.f, 0.f);

	//The closest hit actor.
	Actor* hitActor = nullptr;

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

	Actor* FindHitActor(Actor* findActor);

	//Iterates over every hitActor and returns the closest to 'point' (Good for working with box casts).
	Actor* GetClosestHitActor(const DirectX::XMVECTOR point);

	template <typename T>
	T* GetClosestHitActorAs(const DirectX::XMVECTOR point)
	{
		Actor* actor = GetClosestHitActor(point);
		T* cast = dynamic_cast<T*>(actor);
		return cast;
	}

	void AddActorsToIgnore(std::vector<Actor*>& actors);
	void IgnorePlayer();
	void AddAllRenderStaticMeshesToIgnore();

	//Result is in world space.
	DirectX::XMVECTOR GetHitPosV() const { return XMLoadFloat3(&hitPos); }
	DirectX::XMVECTOR GetNormalV() const { return XMLoadFloat3(&hitNormal); }

	std::vector<Vertex> GetHitVertexFromHitActorMesh();
};

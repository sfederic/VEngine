#pragma once

#include "SpatialComponent.h"
#include "ComponentSystem.h"
#include <functional>

class Actor;
struct HitResult;

class BoxTriggerComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(BoxTriggerComponent);

	//default is green
	XMFLOAT4 renderWireframeColour = XMFLOAT4(0.1f, 0.75f, 0.1f, 1.0f);

	BoxTriggerComponent();
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	//Remember to set arg as bounds in world space
	bool Intersects(const BoundingOrientedBox& boundsInWorldSpaceToCheckAgainst);
	bool IntersectsWithAnyBoundingBoxInWorld();

	bool Contains(XMVECTOR point);
	bool ContainsTarget();
	void SetTargetAsPlayer();
	XMVECTOR GetRandomPointInTrigger();

	void SetExtents(float x, float y, float z);
	XMFLOAT3 GetExtents() const;

	bool QuickInPlaceBoxCast(HitResult& hitResult, bool drawDebug);

	Actor* GetTargetActor() { return targetActor; }
	Actor* SetTargetActor(Actor* actor) { targetActor = actor; }

	void SetTriggerEnterCallback(std::function<void()> callback);
	void SetTriggerExitCallback(std::function<void()> callback);

	template <typename T>
	std::vector<T*> GetAllContainedActors()
	{
		std::vector<T*> containedActors;

		std::vector<T*> actors = World::Get().GetAllActorsOfTypeInWorld<T>();
		for (auto actor : actors)
		{
			if (Contains(actor->GetPositionV()))
			{
				containedActors.emplace_back(actor);
			}
		}

		return containedActors;
	}

private:
	//On trigger enter/exist callback logic for target actor intersection
	void TargetActorIntersectCallbackLogic();

	std::function<void()> triggerEnterCallback;
	std::function<void()> triggerExitCallback;

	Actor* targetActor = nullptr;

	bool targetActorIntersecting = false;
};

#pragma once

#include "SpatialComponent.h"
#include "ComponentSystem.h"

class Actor;
struct HitResult;

class BoxTriggerComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(BoxTriggerComponent)

	//default is green
	XMFLOAT4 renderWireframeColour = XMFLOAT4(0.1f, 0.75f, 0.1f, 1.0f);

	BoxTriggerComponent();
	Properties GetProps() override;

	//Remember to set arg as bounds in world space
	bool Intersects(const BoundingOrientedBox& boundsInWorldSpaceToCheckAgainst);
	bool IntersectsWithAnyBoundingBoxInWorld();

	bool Contains(XMVECTOR point);
	bool ContainsTarget();
	void SetTargetAsPlayer();
	XMVECTOR GetRandomPointInTrigger();

	void SetExtents(float x, float y, float z);
	XMFLOAT3 GetExtents();

	bool QuickInPlaceBoxCast(HitResult& hitResult, bool drawDebug);

	Actor* GetTargetActor() { return targetActor; }
	Actor* SetTargetActor(Actor* actor) { targetActor = actor; }

private:
	Actor* targetActor = nullptr;
};

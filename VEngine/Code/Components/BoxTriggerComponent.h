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

	bool Contains(XMVECTOR point);
	bool ContainsTarget();
	void SetTargetAsPlayer();
	XMVECTOR GetRandomPointInTrigger();
	bool IntersectsWithAnyBoundingBoxInWorld();

	void SetExtents(float x, float y, float z);
	XMFLOAT3 GetExtents();

	bool QuickInPlaceBoxCast(HitResult& hitResult, bool drawDebug);

	Actor* targetActor = nullptr;
};

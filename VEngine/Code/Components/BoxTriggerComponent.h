#pragma once
#include "SpatialComponent.h"
#include "ComponentSystem.h"

class Actor;

struct BoxTriggerComponent : SpatialComponent
{
	COMPONENT_SYSTEM(BoxTriggerComponent)

	Actor* targetActor = nullptr;

	//default is green
	XMFLOAT4 renderWireframeColour = XMFLOAT4(0.1f, 0.75f, 0.1f, 1.0f);

	BoxTriggerComponent();
	virtual Properties GetProps() override;
	bool Contains(XMVECTOR point);
	bool ContainsTarget();
	void SetTargetAsPlayer();
	XMFLOAT3 GetRandomPointInTrigger();
	XMVECTOR GetRandomPointInTriggerRounded();
	bool IntersectsWithAnyBoundingBoxInWorld();
	void SetExtents(float x, float y, float z);
};

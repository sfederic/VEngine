#pragma once
#include "SpatialComponent.h"
#include "ComponentSystem.h"

struct Actor;

struct BoxTriggerComponent : SpatialComponent
{
	COMPONENT_SYSTEM(BoxTriggerComponent)

	Actor* target = nullptr;

	BoxTriggerComponent();
	void Tick(float deltaTime);
	virtual Properties GetProps() override;
	virtual void Create() override;
	bool Contains(XMVECTOR point);
	bool ContainsTarget();
};

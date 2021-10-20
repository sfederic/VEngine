#pragma once
#include "SpatialComponent.h"
#include "ComponentSystem.h"

//An empty spatial component. Used mainly as a basic root compontent for an actor.
struct EmptyComponent : SpatialComponent
{
	COMPONENT_SYSTEM(EmptyComponent)

	EmptyComponent();
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;
};

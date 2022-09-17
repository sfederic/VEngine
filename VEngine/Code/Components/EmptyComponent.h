#pragma once

#include "SpatialComponent.h"
#include "ComponentSystem.h"

//An empty spatial component. Used mainly as a basic root compontent for an actor.
class EmptyComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(EmptyComponent);

	EmptyComponent() {}
	virtual Properties GetProps() override;
};

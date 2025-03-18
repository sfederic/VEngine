#include "Components/ComponentSystemDefine.h"

export module Components.EmptyComponent;

import Components.SpatialComponent;
import Components.ComponentSystem;

//An empty spatial component. Used mainly as a basic root compontent for an actor.
export class EmptyComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(EmptyComponent);

	EmptyComponent() {}
	virtual Properties GetProps() override;
};

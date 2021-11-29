#pragma once
#include "Component.h"
#include "ComponentSystem.h"

struct IntuitionComponent : Component
{
	COMPONENT_SYSTEM(IntuitionComponent);

	IntuitionComponent();
	virtual Properties GetProps() override;
};

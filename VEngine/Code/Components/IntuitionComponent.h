#pragma once
#include "Component.h"
#include "ComponentSystem.h"

struct IntuitionComponent : Component
{
	COMPONENT_SYSTEM(IntuitionComponent);

	std::string name;
	std::string description;
	
	IntuitionComponent();
	virtual Properties GetProps() override;
};

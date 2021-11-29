#pragma once
#include "Component.h"
#include "ComponentSystem.h"

struct IntuitionComponent : Component
{
	COMPONENT_SYSTEM(IntuitionComponent);

	std::string intName;
	std::string intDescription;

	bool addOnInteract = false;
	
	IntuitionComponent();
	virtual Properties GetProps() override;
};

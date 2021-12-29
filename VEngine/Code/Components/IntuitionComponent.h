#pragma once
#include "Component.h"
#include "ComponentSystem.h"

struct Actor;

struct IntuitionComponent : Component
{
	COMPONENT_SYSTEM(IntuitionComponent);

	std::string intuitionName;
	std::string intuitionDescription;

	//The ConditionSystem function to search for when adding to player.
	std::string condition;

	bool addOnInteract = false;
	
	IntuitionComponent();
	virtual Properties GetProps() override;
	bool CreateIntuition(std::string actorAquiredFromName);
};

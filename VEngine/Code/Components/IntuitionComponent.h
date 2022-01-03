#pragma once
#include "Component.h"
#include "ComponentSystem.h"

struct Actor;

struct IntuitionComponent : Component
{
	COMPONENT_SYSTEM(IntuitionComponent);

	std::wstring intuitionName;
	std::wstring intuitionDescription;

	//The ConditionSystem function to search for when adding to player.
	std::string condition;

	//the argument to pass into the above condition function.
	std::string conditionArg;

	bool addOnInteract = false;
	
	IntuitionComponent();
	virtual Properties GetProps() override;
	bool CreateIntuition(std::string actorAquiredFromName);
};

#pragma once
#include "Component.h"
#include "ComponentSystem.h"

//When actors want a condition to check against without the Intuition logic.
//This Component is more like a IntuitionComponent-lite.
struct ConditionComponent : Component
{
	COMPONENT_SYSTEM(ConditionComponent);

	std::string condition;
	std::string conditionArg;

	ConditionComponent();
	virtual Properties GetProps() override;
	bool CheckCondition();
};

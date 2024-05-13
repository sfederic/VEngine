#pragma once

#include "../Component.h"
#include "../ComponentSystem.h"

//When actors want a condition to check against without the Memory logic.
//This Component is more like a MemoryComponent-lite.
class ConditionComponent : public Component
{
public:
	COMPONENT_SYSTEM(ConditionComponent);

	ConditionComponent();
	virtual Properties GetProps() override;
	bool CheckCondition();

	std::string condition;
	std::string conditionArg;
};

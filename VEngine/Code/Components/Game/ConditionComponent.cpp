#include "vpch.h"
#include "ConditionComponent.h"
#include "Gameplay/ConditionSystem.h"

ConditionComponent::ConditionComponent()
{
}

Properties ConditionComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = "ConditionComponent";
	props.AddProp(condition);
	props.AddProp(conditionArg);
	return props;
}

bool ConditionComponent::CheckCondition()
{
	auto conditionFunction = conditionSystem.FindConditionAllowNull(condition);
	if (conditionFunction)
	{
		return conditionFunction(conditionArg);
	}

	return false;
}

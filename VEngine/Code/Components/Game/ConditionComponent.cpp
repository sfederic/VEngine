
#include "ConditionComponent.h"
#include "Gameplay/ConditionSystem.h"

ConditionComponent::ConditionComponent()
{
}

Properties ConditionComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Condition", &condition);
	props.Add("Condition Arg", &conditionArg);
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

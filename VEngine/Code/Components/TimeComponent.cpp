#include "TimeComponent.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/ConditionSystem.h"

TimeComponent::TimeComponent()
{
}

Properties TimeComponent::GetProps()
{
    Properties props("TimeComponent");
    props.AddProp(activeBeginHour);
    props.AddProp(activeEndHour);
    props.AddProp(alwaysActive);
    props.AddProp(condition);
    props.AddProp(conditionArg);
    return props;
}

bool TimeComponent::CheckIfActiveAtCurrentTime()
{
    if (alwaysActive)
    {
        if (!condition.empty())
        {
            return CheckIfActiveFromCondition();
        }

        return true;
    }

    const int currentHour = GameInstance::currentHour;

    if (currentHour >= activeBeginHour && currentHour <= activeEndHour)
    {
        if (!condition.empty())
        {
            return CheckIfActiveFromCondition();
        }

        return true;
    }

    return false;
}

bool TimeComponent::CheckIfActiveFromCondition()
{
    auto foundConditionFunction = conditionSystem.FindCondition(condition);
    return foundConditionFunction(conditionArg);
}

#include "vpch.h"
#include "TimeComponent.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/ConditionSystem.h"
#include "Log.h"

TimeComponent::TimeComponent()
{
}

Properties TimeComponent::GetProps()
{
    auto props = __super::GetProps();
    props.title = "TimeComponent";
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
    auto foundConditionFunction = conditionSystem.FindConditionAllowNull(condition);
    if (foundConditionFunction)
    {
        if (foundConditionFunction(conditionArg))
        {
            return true;
        }
        else
        {
            Log("Condition [%s] failed with arg [%s]", condition.c_str(), conditionArg.c_str());
        }
    }

    Log("Condition [%s] not found for TimeComponent.", condition.c_str());
    return false;
}

#include "TimeComponent.h"
#include "Gameplay/GameInstance.h"

TimeComponent::TimeComponent()
{
}

Properties TimeComponent::GetProps()
{
    Properties props("TimeOfDayComponent");
    props.AddProp(activeStartHour);
    props.AddProp(activeStartMinute);
    props.AddProp(activeEndHour);
    props.AddProp(activeEndMinute);
    return props;
}

bool TimeComponent::CheckIfActiveAtCurrentTime()
{
    const int currentHour = GameInstance::currentHour;
    const int currentMinute = GameInstance::currentMinute;

    if (activeStartHour >= currentHour && activeEndHour >= currentHour)
    {
        if (activeStartMinute >= currentMinute && activeEndMinute >= currentMinute)
        {
            return true;
        }
    }

    return false;
}

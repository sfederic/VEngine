#include "TimeOfDayComponent.h"
#include "Gameplay/GameInstance.h"

TimeOfDayComponent::TimeOfDayComponent()
{
}

Properties TimeOfDayComponent::GetProps()
{
    Properties props("TimeOfDayComponent");
    props.AddProp(activeStartHour);
    props.AddProp(activeStartMinute);
    props.AddProp(activeEndHour);
    props.AddProp(activeEndMinute);
    return props;
}

bool TimeOfDayComponent::CheckIfActiveAtCurrentTime()
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

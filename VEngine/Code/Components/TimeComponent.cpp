#include "TimeComponent.h"
#include "Gameplay/GameInstance.h"

TimeComponent::TimeComponent()
{
}

Properties TimeComponent::GetProps()
{
    Properties props("TimeOfDayComponent");
    props.AddProp(activeBeginHour);
    props.AddProp(activeBeginMinute);
    props.AddProp(activeEndHour);
    props.AddProp(activeEndMinute);
    return props;
}

bool TimeComponent::CheckIfActiveAtCurrentTime()
{
    const int currentHour = GameInstance::currentHour;
    const int currentMinute = GameInstance::currentMinute;

    if (activeBeginHour >= currentHour && activeEndHour >= currentHour)
    {
        if (activeBeginMinute >= currentMinute && activeEndMinute >= currentMinute)
        {
            return true;
        }
    }

    return false;
}

#include "TimeComponent.h"
#include "Gameplay/GameInstance.h"

TimeComponent::TimeComponent()
{
}

Properties TimeComponent::GetProps()
{
    Properties props("TimeOfDayComponent");
    props.AddProp(activeBeginHour);
    props.AddProp(activeEndHour);
    return props;
}

bool TimeComponent::CheckIfActiveAtCurrentTime()
{
    const int currentHour = GameInstance::currentHour;

    if (currentHour >= activeBeginHour && currentHour <= activeEndHour)
    {
        return true;
    }

    return false;
}

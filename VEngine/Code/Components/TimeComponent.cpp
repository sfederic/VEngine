#include "TimeComponent.h"
#include "Gameplay/GameInstance.h"

TimeComponent::TimeComponent()
{
}

Properties TimeComponent::GetProps()
{
    Properties props("TimeComponent");
    props.AddProp(activeBeginHour);
    props.AddProp(activeEndHour);
    props.AddProp(alwaysActive);
    return props;
}

bool TimeComponent::CheckIfActiveAtCurrentTime()
{
    if (alwaysActive)
    {
        return true;
    }

    const int currentHour = GameInstance::currentHour;

    if (currentHour >= activeBeginHour && currentHour <= activeEndHour)
    {
        return true;
    }

    return false;
}

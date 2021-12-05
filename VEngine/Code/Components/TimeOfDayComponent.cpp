#include "TimeOfDayComponent.h"

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

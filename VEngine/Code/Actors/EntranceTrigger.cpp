#include "EntranceTrigger.h"
#include "Components/BoxTriggerComponent.h"

EntranceTrigger::EntranceTrigger()
{
    trigger = BoxTriggerComponent::system.Add(this);
}

Properties EntranceTrigger::GetProps()
{
    return Properties();
}

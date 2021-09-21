#include "EntranceTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "GameUtils.h"

EntranceTrigger::EntranceTrigger()
{
    trigger = BoxTriggerComponent::system.Add(this);
    rootComponent = trigger;
}

void EntranceTrigger::Start()
{
    trigger->target = (Actor*)GameUtils::GetPlayer();
}

void EntranceTrigger::Tick(double deltaTime)
{
    XMVECTOR targetPos = trigger->target->GetPositionVector();
    if (trigger->boundingBox.Contains(targetPos))
    {
    }
}

Properties EntranceTrigger::GetProps()
{
    return Actor::GetProps();
}

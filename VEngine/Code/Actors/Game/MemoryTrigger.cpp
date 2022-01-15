#include "MemoryTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Gameplay/GameUtils.h"
#include "Input.h"

MemoryTrigger::MemoryTrigger()
{
    trigger = BoxTriggerComponent::system.Add(this);
}

void MemoryTrigger::Start()
{
    trigger->targetActor = (Actor*)GameUtils::GetPlayer();
}

void MemoryTrigger::Tick(float deltaTime)
{
    if (trigger->ContainsTarget())
    {
        if (Input::GetKeyUp(Keys::Down))
        {
            
        }
    }
}

Properties MemoryTrigger::GetProps()
{
    return __super::GetProps();
}

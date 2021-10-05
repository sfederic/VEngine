#include "EntranceTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "GameUtils.h"
#include "Input.h"
#include "FileSystem.h"

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
    if (trigger->Contains(targetPos))
    {
        if (Input::GetKeyUp(Keys::Down))
        {
            //Load new world
            fileSystem.LoadWorld(levelToMoveTo);
        }
    }
}

Properties EntranceTrigger::GetProps()
{
    auto props = Actor::GetProps();
    props.title = "EntranceTrigger";
    props.Add("Level Name", &levelToMoveTo);
    return props;
}

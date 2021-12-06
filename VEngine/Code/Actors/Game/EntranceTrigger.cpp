#include "EntranceTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/GameInstance.h"
#include "Input.h"
#include "FileSystem.h"
#include "UI/InteractWidget.h"

EntranceTrigger::EntranceTrigger()
{
    trigger = BoxTriggerComponent::system.Add(this);
    rootComponent = trigger;

    openText = "Open";
}

void EntranceTrigger::Start()
{
    interactWidget = CreateWidget<InteractWidget>();
    interactWidget->SetText(openText);

    trigger->target = (Actor*)GameUtils::GetPlayer();
}

void EntranceTrigger::Tick(float deltaTime)
{
    XMVECTOR targetPos = trigger->target->GetPositionVector();

    if (trigger->ContainsTarget() && isEntraceActive)
    {
        interactWidget->AddToViewport();

        if (Input::GetKeyUp(Keys::Down))
        {
            //Load new world
            GameInstance::ProgressTime();
            fileSystem.LoadWorld(levelToMoveTo);
            return;
        }
    }
    else
    {
        interactWidget->RemoveFromViewport();
    }
}

Properties EntranceTrigger::GetProps()
{
    auto props = Actor::GetProps();
    props.title = "EntranceTrigger";
    props.Add("Level Name", &levelToMoveTo);
    props.Add("Entrance Active", &isEntraceActive);
    props.Add("Open Text", &openText);
    return props;
}

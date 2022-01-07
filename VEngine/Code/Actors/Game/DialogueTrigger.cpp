#include "DialogueTrigger.h"
#include "Components/DialogueComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "UI/DialogueWidget.h"
#include "Timer.h"

DialogueTrigger::DialogueTrigger()
{
    boxTriggerComponent = BoxTriggerComponent::system.Add(this);
    rootComponent = boxTriggerComponent;

    dialogueComponent = DialogueComponent::system.Add(this);
}

void DialogueTrigger::Start()
{
    boxTriggerComponent->SetTargetAsPlayer();

    if (playOnSpawn)
    {
        NextLine();
    }
}

void DialogueTrigger::Tick(float deltaTime)
{
    if (boxTriggerComponent->ContainsTarget())
    {
        if (playOnTriggerOverlap)
        {
            playOnTriggerOverlap = false;
            NextLine();
        }
    }
}

Properties DialogueTrigger::GetProps()
{
    auto props = __super::GetProps();
    props.AddProp(playOnSpawn);
    props.AddProp(playOnTriggerOverlap);
    return props;
}

void DialogueTrigger::NextLine()
{
    if (dialogueFinished)
    {
        return;
    }

    if (dialogueComponent->previousActiveDialogueWidget)
    {
        dialogueComponent->previousActiveDialogueWidget->RemoveFromViewport();
    }

    dialogueComponent->ConversationShowTextAtActor();

    if (dialogueComponent->ConversationNextLine())
    {
        Timer::SetTimer(4.f, std::bind(&DialogueTrigger::NextLine, this));
    }
    else
    {
        dialogueFinished = true;
    }
}

#include "vpch.h"
#include "DialogueTrigger.h"
#include "Components/Game/DialogueComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "UI/Game/DialogueWidget.h"
#include "Core/Timer.h"
#include "Core/Input.h"

DialogueTrigger::DialogueTrigger()
{
    boxTriggerComponent = BoxTriggerComponent::system.Add("BoxTrigger", this);
    boxTriggerComponent->renderWireframeColour = XMFLOAT4(0.1f, 0.45f, 0.9f, 1.f);
    rootComponent = boxTriggerComponent;

    dialogueComponent = DialogueComponent::system.Add("Dialogue", this);
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
        if (playOnTriggerOverlap && firstTimePlaying) //Play dialogue on overlap
        {
            firstTimePlaying = false;
            NextLine();
            return;
        }
        else if (Input::GetKeyUp(Keys::Down)) //Play dialogue on input
        {
            firstTimePlaying = false;
            NextLine();
            return;
        }
    }
}

Properties DialogueTrigger::GetProps()
{
    auto props = __super::GetProps();
    props.Add("PlayOnSpawn", &playOnSpawn);
    props.Add("PlayOnTriggerOverlap", &playOnTriggerOverlap);
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

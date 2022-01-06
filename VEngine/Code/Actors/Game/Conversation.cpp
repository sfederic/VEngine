#include "Conversation.h"
#include "Components/DialogueComponent.h"
#include "Components/EmptyComponent.h"
#include "UI/DialogueWidget.h"
#include "TimerSystem.h"

Conversation::Conversation()
{
    rootComponent = EmptyComponent::system.Add(this);

    dialogueComponent = DialogueComponent::system.Add(this);
}

void Conversation::Start()
{
    if (playOnSpawn)
    {
        NextLine();
    }
}

void Conversation::Tick(float deltaTime)
{
}

Properties Conversation::GetProps()
{
    auto props = __super::GetProps();
    props.AddProp(playOnSpawn);
    return props;
}

void Conversation::NextLine()
{
    if (dialogueComponent->previousActiveDialogueWidget)
    {
        dialogueComponent->previousActiveDialogueWidget->RemoveFromViewport();
    }

    dialogueComponent->ConversationShowTextAtActor();

    if (dialogueComponent->ConversationNextLine())
    {
        timerSystem.SetTimer(4.f, std::bind(&Conversation::NextLine, this));
    }
}

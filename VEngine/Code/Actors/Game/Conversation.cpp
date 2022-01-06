#include "Conversation.h"
#include "Components/DialogueComponent.h"
#include "Components/EmptyComponent.h"
#include "TimerSystem.h"

Conversation::Conversation()
{
    rootComponent = EmptyComponent::system.Add(this);

    dialogueComponent = DialogueComponent::system.Add(this);
}

void Conversation::Start()
{
    NextLine();
}

void Conversation::Tick(float deltaTime)
{
}

Properties Conversation::GetProps()
{
    auto props = __super::GetProps();
    return props;
}

void Conversation::NextLine()
{
    dialogueComponent->ShowTextAtActor();
    dialogueComponent->NextLine();
    timerSystem.SetTimer(4.f, std::bind(&Conversation::NextLine, this));
}

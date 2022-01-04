#include "NPC.h"
#include "VString.h"
#include "TimerSystem.h"
#include "UI/DialogueWidget.h"
#include "Components/DialogueComponent.h"

NPC::NPC()
{
    isDestructible = true;
}

void NPC::Start()
{
    __super::Start();
}

void NPC::Tick(float deltaTime)
{
    __super::Tick(deltaTime);
}

Properties NPC::GetProps()
{
    Properties props = __super::GetProps();
    props.title = "NPC";
    return props;
}

void NPC::QuickTalkTo()
{
    if (isQuickDialogueActive) return;

    isQuickDialogueActive = true;

    dialogueComponent->dialogueWidget->dialogueText = interactText;
    dialogueComponent->AddToViewport();

    timerSystem.SetTimer(3.0f, std::bind(&NPC::EndQuickTalkTo, this));
}

void NPC::EndQuickTalkTo()
{
    isQuickDialogueActive = false;
    dialogueComponent->RemoveFromViewport();
}

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

    if (!spawnText.empty())
    {
        if (isQuickDialogueActive) return;

        spawnTextWidget = CreateWidget<DialogueWidget>();
        spawnTextWidget->dialogueText = spawnText;
        spawnTextWidget->AddToViewport(5.0f);
    }
}

void NPC::Tick(float deltaTime)
{
    __super::Tick(deltaTime);

    spawnTextWidget->pos = GetHomogeneousPositionVector();
}

Properties NPC::GetProps()
{
    Properties props = __super::GetProps();
    props.title = "NPC";
    props.AddProp(spawnText);
    return props;
}

void NPC::QuickTalkTo()
{
    spawnTextWidget->RemoveFromViewport();

    if (isQuickDialogueActive) return;

    isQuickDialogueActive = true;

    dialogueComponent->dialogueWidget->dialogueText = interactText;
    dialogueComponent->AddToViewport();

    timerSystem.SetTimer(5.0f, std::bind(&NPC::EndQuickTalkTo, this));
}

void NPC::EndQuickTalkTo()
{
    isQuickDialogueActive = false;
    dialogueComponent->RemoveFromViewport();
}

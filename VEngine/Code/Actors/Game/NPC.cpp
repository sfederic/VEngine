#include "vpch.h"
#include "NPC.h"
#include "Core/Timer.h"
#include "UI/Game/DialogueWidget.h"
#include "UI/UISystem.h"
#include "Components/Game/DialogueComponent.h"

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

        spawnTextWidget = UISystem::CreateWidget<DialogueWidget>();
        spawnTextWidget->dialogueText = spawnText;
        spawnTextWidget->AddToViewport(5.0f);
    }
}

void NPC::Tick(float deltaTime)
{
    if (spawnTextWidget)
    {
        spawnTextWidget->worldPosition = GetHomogeneousPositionV();
    }

    if (isQuickDialogueActive)
    {
        quickTalkTimer += deltaTime;
        if (quickTalkTimer > 4.0f)
        {
            EndQuickTalkTo();
            quickTalkTimer = 0.f;
        }
    }

    __super::Tick(deltaTime);
}

Properties NPC::GetProps()
{
    Properties props = __super::GetProps();
    props.title = "NPC";
    props.Add("Spawn Text", &spawnText);
    props.Add("BattleStartText", &battleStartText);
    return props;
}

void NPC::QuickTalkTo()
{
    spawnTextWidget->RemoveFromViewport();

    if (isQuickDialogueActive) return;

    isQuickDialogueActive = true;

    dialogueComponent->dialogueWidget->dialogueText = interactText;
    dialogueComponent->AddToViewport();
}

void NPC::BattleStartDialogue()
{
    if (battleStartText.empty())
    {
        return;
    }

    spawnTextWidget->RemoveFromViewport();

    if (isQuickDialogueActive)
    {
        return;
    }

    isQuickDialogueActive = true;

    dialogueComponent->dialogueWidget->dialogueText = battleStartText;
    dialogueComponent->AddToViewport();
}

void NPC::EndQuickTalkTo()
{
    isQuickDialogueActive = false;
    dialogueComponent->RemoveFromViewport();
}

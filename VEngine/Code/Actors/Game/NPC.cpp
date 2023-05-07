#include "vpch.h"
#include "NPC.h"
#include "EntranceTrigger.h"
#include "Grid.h"
#include "Core/Timer.h"
#include "UI/Game/DialogueWidget.h"
#include "UI/UISystem.h"
#include "Components/Game/DialogueComponent.h"

NPC::NPC()
{
    isDestructible = true;
}

void NPC::Create()
{
    moveSpeed = 3.f;

    canBeMovedInLink = false;
    canBeRotatedInLink = false;
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
    TryToEscapeToExit();

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

void NPC::EndQuickTalkTo()
{
    isQuickDialogueActive = false;
    dialogueComponent->RemoveFromViewport();
}

void NPC::TryToEscapeToExit()
{
    if (entranceReachableForEscape)
    {
        return;
    }

    auto entrance = EntranceTrigger::system.GetFirstActor();
    int xIndex = (int)std::round(entrance->GetPosition().x);
    int yIndex = (int)std::round(entrance->GetPosition().z);
    auto grid = Grid::system.GetOnlyActor();
    auto entranceNode = grid->GetNode(xIndex, yIndex);
    
    MoveToNode(entranceNode);

    for (auto pathNode : pathNodes)
    {
        if (pathNode->Equals(entranceNode))
        {
            entranceReachableForEscape = true;
        }
    }
    
    if (!entranceReachableForEscape)
    {
        pathNodes.clear();
    }
}

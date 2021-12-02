#include "NPC.h"
#include "Components/DialogueComponent.h"
#include "Gameplay/GameUtils.h"
#include "VString.h"
#include "UI/UISystem.h"
#include "UI/DialogueWidget.h"

NPC::NPC()
{
    dialogueComponent = DialogueComponent::system.Add(this);

    isDestructible = true;
}

void NPC::Start()
{
    __super::Start();
}

void NPC::Tick(float deltaTime)
{
    __super::Tick(deltaTime);

    dialogueComponent->SetPosition(GetHomogeneousPositionVector());
}

Properties NPC::GetProps()
{
    Properties props = __super::GetProps();
    props.title = "NPC";
    return props;
}

void NPC::QuickTalkTo()
{
    dialogueComponent->dialogueWidget->dialogueText = stows(interactText);
    dialogueComponent->AddToViewport();
}

#include "NPC.h"
#include "Components/WidgetComponent.h"
#include "Components/DialogueComponent.h"
#include "Gameplay/GameUtils.h"
#include "VString.h"
#include "UI/UISystem.h"
#include "UI/DialogueWidget.h"

NPC::NPC()
{
    dialogue = DialogueComponent::system.Add(this);

    dialogueWidget = WidgetComponent::system.Add(this);
    dialogueWidget->widget = uiSystem.CreateWidget<DialogueWidget>();

    isDestructible = true;
}

void NPC::Start()
{
    __super::Start();
}

void NPC::Tick(float deltaTime)
{
    __super::Tick(deltaTime);

    dialogueWidget->widget->pos = GetHomogeneousPositionVector();
}

Properties NPC::GetProps()
{
    Properties props = __super::GetProps();
    props.title = "NPC";
    return props;
}

void NPC::TalkTo()
{
    auto player = GameUtils::GetPlayer();

    auto widget = uiSystem.CreateWidget<DialogueWidget>();
    widget->pos = GetHomogeneousPositionVector();
    widget->AddToViewport();
}

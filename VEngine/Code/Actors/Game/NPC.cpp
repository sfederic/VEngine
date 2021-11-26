#include "NPC.h"
#include "Components/MeshComponent.h"
#include "Components/DialogueComponent.h"
#include "GameUtils.h"
#include "VString.h"
#include "UI/Widget.h"

NPC::NPC()
{
    mesh = MeshComponent::system.Add(this, MeshComponent("cube.fbx", "test.png"));
    rootComponent = mesh;

    dialogue = DialogueComponent::system.Add(this);
}

void NPC::Start()
{

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

    auto widget = new Widget();
    widget->pos = GetHomogeneousPositionVector();
    widget->displayText = stows(dialogue->dialogue.data[0].text);
    widget->AddToViewport();
}

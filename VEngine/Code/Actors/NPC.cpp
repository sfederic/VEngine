#include "NPC.h"
#include "Components/MeshComponent.h"
#include "Components/DialogueComponent.h"

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
    Properties props = Actor::GetProps();
    props.title = "NPC";
    return props;
}

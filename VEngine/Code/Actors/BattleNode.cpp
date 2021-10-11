#include "BattleNode.h"
#include "Components/MeshComponent.h"

BattleNode::BattleNode()
{
    nodeMesh = MeshComponent::system.Add(this, MeshComponent("plane.fbx", "test.png"));
    rootComponent = nodeMesh;
}

Properties BattleNode::GetProps()
{
    return Properties();
}

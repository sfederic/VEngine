#include "BattleNode.h"
#include "Components/InstanceMeshComponent.h"
#include "Render/RenderUtils.h"
#include "Render/Material.h"

BattleNode::BattleNode()
{
    nodeMesh = InstanceMeshComponent::system.Add(this, InstanceMeshComponent(3, "node.fbx", "test.png"));
    nodeMesh->material->shaderFilename = "InstanceShader.hlsl";
    rootComponent = nodeMesh;
}

void BattleNode::Start()
{
    for (int i = 0; i < nodeMesh->GetInstanceCount(); i++)
    {
        XMMATRIX rootWorldMatrix = rootComponent->GetWorldMatrix();
        InstanceData instanceData = {};
        instanceData.world = rootWorldMatrix;
        nodeMesh->instanceData.push_back(instanceData);
        nodeMesh->instanceData[i].world.r[3].m128_f32[2] = i * 2;
    }
}

Properties BattleNode::GetProps()
{
    auto props = Actor::GetProps();
    props.title = "BattleNode";
    return props;
}

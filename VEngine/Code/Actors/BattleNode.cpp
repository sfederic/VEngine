#include "BattleNode.h"
#include "Components/InstanceMeshComponent.h"
#include "Render/RenderUtils.h"
#include "Render/Material.h"
#include "Physics/Raycast.h"
#include "VMath.h"

BattleNode::BattleNode()
{
    int meshInstanceCount = 5 * 5;

    nodeMesh = InstanceMeshComponent::system.Add(
        this, InstanceMeshComponent(meshInstanceCount, "node.fbx", "test.png"));

    nodeMesh->material->shaderFilename = "InstanceShader.hlsl";
    rootComponent = nodeMesh;
}

void BattleNode::Start()
{
    XMMATRIX rootWorldMatrix = rootComponent->GetWorldMatrix();
    XMVECTOR rayOrigin = XMVectorZero();

    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            rayOrigin = XMVectorSet(x, 10.f, y, 1.f);

            Ray ray = {};
            ray.actorsToIgnore.push_back(this);
            if (Raycast(ray, rayOrigin, -VMath::XMVectorUp(), 20.0f))
            {
                InstanceData instanceData = {};
                instanceData.world = rootWorldMatrix;

                //Scale the node down a little
                XMMATRIX scaleMatrix = XMMatrixScaling(0.9f, 0.9f, 0.9f);
                instanceData.world *= scaleMatrix;

                nodeMesh->instanceData.push_back(instanceData);

                //Position the node at the raycast's hitpos
                XMFLOAT3 hitPos = ray.hitPos;
                hitPos.y += 0.1f;
                XMVECTOR hitPosVector = XMLoadFloat3(&hitPos);
                hitPosVector.m128_f32[3] = 1.0f;

                nodeMesh->instanceData.back().world.r[3] = hitPosVector;

            }
        }
    }
}

Properties BattleNode::GetProps()
{
    auto props = Actor::GetProps();
    props.title = "BattleNode";
    props.Add("Size X", &sizeX);
    props.Add("Size Y", &sizeY);
    return props;
}

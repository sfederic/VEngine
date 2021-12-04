#include "BattleGrid.h"
#include "Components/InstanceMeshComponent.h"
#include "Render/RenderUtils.h"
#include "Render/Material.h"
#include "Render/Renderer.h"
#include "Physics/Raycast.h"
#include "VMath.h"
#include "Actors/Game/GridActor.h"

BattleGrid::BattleGrid()
{
    nodeMesh = InstanceMeshComponent::system.Add(
        this, InstanceMeshComponent(1, "node.fbx", "test.png"));

    nodeMesh->material->shaderData.filename = "InstanceShader.hlsl";
    rootComponent = nodeMesh;
}

void BattleGrid::Start()
{
    //This is all pretty lazy, but I need the sizeX and sizeY to determine the buffer sizes on gameplay start
    //and can't do it in the InstanceMeshComponent constructors.
    if (nodeMesh->structuredBuffer)
    {
        nodeMesh->structuredBuffer->Release();
    }
    if (nodeMesh->srv)
    {
        nodeMesh->srv->Release();
    }


    //Set the mesh count as 1 and empty the data just to put dummy data into the buffers
    nodeMesh->instanceData.clear();
    nodeMesh->instanceData.push_back(InstanceData());

    int meshInstanceCount = sizeX * sizeY;
    nodeMesh->SetInstanceCount(meshInstanceCount);

    //Re-setup shader buffers
    nodeMesh->structuredBuffer = RenderUtils::CreateStructuredBuffer(sizeof(InstanceData) * meshInstanceCount,
        sizeof(InstanceData), nodeMesh->instanceData.data());

    nodeMesh->srv = RenderUtils::CreateSRVForMeshInstance(nodeMesh->structuredBuffer, meshInstanceCount);


    XMMATRIX rootWorldMatrix = rootComponent->GetWorldMatrix();
    XMVECTOR rayOrigin = XMVectorZero();

    nodeMesh->instanceData.clear();

    for (int x = 0; x < sizeX; x++)
    {
        //create grid row
        rows.push_back(GridRow());

        for (int y = 0; y < sizeY; y++)
        {
            rayOrigin = XMVectorSet(x, 10.f, y, 1.f);

            //Set instance model matrix
            InstanceData instanceData = {};
            instanceData.world = rootWorldMatrix;

            //create grid node in row
            GridNode node = GridNode(x, y, nodeMesh->instanceData.size());

            //raycast against the world to set node position
            Ray ray(this);
            if (Raycast(ray, rayOrigin, -VMath::XMVectorUp(), 20.0f))
            {
                //Scale the node down a little so that nodes aren't touching
                XMMATRIX scaleMatrix = XMMatrixScaling(0.9f, 0.9f, 0.9f);
                instanceData.world *= scaleMatrix;

                //Position the node at the raycast's hitpos
                XMFLOAT3 hitPos = ray.hitPos;
                hitPos.y += 0.1f;
                XMVECTOR hitPosVector = XMLoadFloat3(&hitPos);
                hitPosVector.m128_f32[3] = 1.0f;

                instanceData.world.r[3] = hitPosVector;

                node.active = true;

                if (ray.hitActor)
                {
                    auto gridActor = dynamic_cast<GridActor*>(ray.hitActor);
                    if (gridActor)
                    {
                        if (gridActor->isGridObstacle)
                        {
                            //Mul by empty scale matrix to make the node invisible in-scene
                            XMMATRIX emptyScaleMatrix = XMMatrixScaling(0.f, 0.f, 0.f);
                            instanceData.world *= emptyScaleMatrix;

                            node.active = false;
                        }
                    }
                }
            }
            else
            {
                //Mul by empty scale matrix to make the node invisible in-scene
                XMMATRIX emptyScaleMatrix = XMMatrixScaling(0.f, 0.f, 0.f);
                instanceData.world *= emptyScaleMatrix;

                node.active = false;
            }

            nodeMesh->instanceData.push_back(instanceData);

            //Add node to column
            rows[x].Add(node);
        }
    }
}

Properties BattleGrid::GetProps()
{
    auto props = Actor::GetProps();
    props.title = "BattleNode";
    props.Add("Size X", &sizeX);
    props.Add("Size Y", &sizeY);
    return props;
}

GridNode* BattleGrid::GetNode(int x, int y)
{
    return &rows[x].columns[y];
}

void BattleGrid::GetNeighbouringNodes(GridNode* centerNode, std::vector<GridNode*>& outNodes)
{
    int currentX = centerNode->xIndex;
    int currentY = centerNode->yIndex;

    //+X
    if (currentX < (sizeX - 1))
    {
        GridNode& node = rows[currentX + 1].columns[currentY];
        if (!node.closed && node.active)
        {
            node.closed = true;
            node.parentNode = centerNode;
            outNodes.push_back(&node);
        }
    }

    //-X
    if (currentX > 0)
    {
        GridNode& node = rows[currentX - 1].columns[currentY];
        if (!node.closed && node.active)
        {
            node.closed = true;
            node.parentNode = centerNode;
            outNodes.push_back(&node);
        }
    }

    //+Y
    if (currentY < (sizeY - 1))
    {
        GridNode& node = rows[currentX].columns[currentY + 1];
        if (!node.closed && node.active)
        {
            node.closed = true;
            node.parentNode = centerNode;
            outNodes.push_back(&node);
        }
    }

    //-Y
    if (currentY > 0)
    {
        GridNode& node = rows[currentX].columns[currentY - 1];
        if (!node.closed && node.active)
        {
            node.closed = true;
            node.parentNode = centerNode;
            outNodes.push_back(&node);
        }
    }
}

#include "Grid.h"
#include "Components/InstanceMeshComponent.h"
#include "Components/TimeComponent.h"
#include "Render/RenderUtils.h"
#include "Render/Material.h"
#include "Render/Renderer.h"
#include "Physics/Raycast.h"
#include "VMath.h"
#include "GridActor.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/BattleSystem.h"

Grid::Grid()
{
    nodeMesh = InstanceMeshComponent::system.Add(
        this, InstanceMeshComponent(1, "node.fbx", "test.png"));

    nodeMesh->material->shaderData.filename = "InstanceShader.hlsl";
    rootComponent = nodeMesh;
}

//Grid::Awake() can also be used to reset all node world positions during gameplay
void Grid::Awake()
{
    nodeMesh->ReleaseBuffers();

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

    //Deactive gridactors that aren't active at current time
    {
        auto gridActors = world.GetAllActorsOfTypeInWorld<GridActor>();
        for (auto gridActor : gridActors)
        {
            gridActor->EnableBasedOnTime();
        }
    }

    //Ignore player and units
    Ray ray(this);
    ray.ignoreLayer = CollisionLayers::Editor;
    ray.actorsToIgnore.push_back((Actor*)GameUtils::GetPlayer());
    auto gridActors = world.GetAllActorsOfTypeAsActor<GridActor>();
    for (auto gridActor : gridActors)
    {
        auto castActor = (GridActor*)gridActor;
        if (castActor)
        {
            if (castActor->isGridObstacle)
            {
                continue;
            }
        }

        ray.actorsToIgnore.push_back(gridActor);
    }

    rows.clear();

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

            instanceData.colour = GridNode::normalColour;

            //raycast against the world to set node position
            if (Raycast(ray, rayOrigin, -VMath::XMVectorUp(), 20.0f))
            {
                //Scale the node down to nothing
                XMMATRIX scaleMatrix = XMMatrixScaling(0.f, 0.f, 0.f);
                instanceData.world *= scaleMatrix;

                //Position the node at the raycast's hitpos
                XMFLOAT3 hitPos = ray.hitPos;
                hitPos.y += 0.1f;
                XMVECTOR hitPosVector = XMLoadFloat3(&hitPos);
                hitPosVector.m128_f32[3] = 1.0f;

                //set the y-pos for the node
                node.worldPosition.y = hitPos.y + 0.4f;

                instanceData.world.r[3] = hitPosVector;

                node.active = true;

                if (ray.hitActor)
                {
                    auto meshes = ray.hitActor->GetComponentsOfType<MeshComponent>();
                    for (auto mesh : meshes)
                    {
                        if (mesh->gridObstacle)
                        {
                            node.active = false;
                        }
                    }

                    auto gridActor = dynamic_cast<GridActor*>(ray.hitActor);
                    if (gridActor)
                    {
                        if (gridActor->isGridObstacle)
                        {
                            node.active = false;
                        }
                    }
                }
            }
            else
            {
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

void Grid::Start()
{

}

void Grid::LateStart()
{
    if (startBattleOnLoad)
    {
        SetActive(true);
        battleSystem.StartBattle();
    }
    else
    {
        SetActive(false);
    }
}

void Grid::Tick(float deltaTime)
{
    switch(lerpValue)
    {
    case LerpValue::LerpIn:
        LerpInNodes(deltaTime);
        break;

    case LerpValue::LerpOut:
        LerpOutNodes(deltaTime);
        break;
    }
}

Properties Grid::GetProps()
{
    auto props = __super::GetProps();
    props.title = "BattleNode";
    props.Add("Size X", &sizeX);
    props.Add("Size Y", &sizeY);
    props.AddProp(startBattleOnLoad);
    return props;
}

GridNode* Grid::GetNode(int x, int y)
{
    assert(x < rows.size());
    assert(y < rows[x].columns.size());
    return &rows[x].columns[y];
}

void Grid::GetNeighbouringNodes(GridNode* centerNode, std::vector<GridNode*>& outNodes)
{
    int currentX = centerNode->xIndex;
    int currentY = centerNode->yIndex;

    //+X
    if (currentX < (sizeX - 1))
    {
        GridNode& node = rows[currentX + 1].columns[currentY];
        if (!node.closed && node.active)
        {
            if (node.worldPosition.y < (centerNode->worldPosition.y + Grid::maxHeightMove))
            {
                node.closed = true;
                node.parentNode = centerNode;
                outNodes.push_back(&node);
            }
        }
    }

    //-X
    if (currentX > 0)
    {
        GridNode& node = rows[currentX - 1].columns[currentY];
        if (!node.closed && node.active)
        {
            if (node.worldPosition.y < (centerNode->worldPosition.y + Grid::maxHeightMove))
            {
                node.closed = true;
                node.parentNode = centerNode;
                outNodes.push_back(&node);
            }
        }
    }

    //+Y
    if (currentY < (sizeY - 1))
    {
        GridNode& node = rows[currentX].columns[currentY + 1];
        if (!node.closed && node.active)
        {
            if (node.worldPosition.y < (centerNode->worldPosition.y + Grid::maxHeightMove))
            {
                node.closed = true;
                node.parentNode = centerNode;
                outNodes.push_back(&node);
            }
        }
    }

    //-Y
    if (currentY > 0)
    {
        GridNode& node = rows[currentX].columns[currentY - 1];
        if (!node.closed && node.active)
        {
            if (node.worldPosition.y < (centerNode->worldPosition.y + Grid::maxHeightMove))
            {
                node.closed = true;
                node.parentNode = centerNode;
                outNodes.push_back(&node);
            }
        }
    }
}

void Grid::ResetAllNodes()
{
    for (auto& row : rows)
    {
        for (auto& node : row.columns)
        {
            node.ResetValues();
        }
    }
}

void Grid::LerpInNodes(float deltaTime)
{
    const float lerpSpeed = 4.5f;

    for (auto& row : rows)
    {
        for (auto& node : row.columns)
        {
            if (!node.active || node.preview)
            {
                continue;
            }

            auto& data = nodeMesh->instanceData[node.instancedMeshIndex];
            data.world.r[0].m128_f32[0] = std::lerp(data.world.r[0].m128_f32[0], 0.f, deltaTime * lerpSpeed);
            data.world.r[1].m128_f32[1] = std::lerp(data.world.r[1].m128_f32[1], 0.f, deltaTime * lerpSpeed);
            data.world.r[2].m128_f32[2] = std::lerp(data.world.r[2].m128_f32[2], 0.f, deltaTime * lerpSpeed);
        }
    }
}

void Grid::LerpOutNodes(float deltaTime)
{
    const float lerpSpeed = 4.5f;

    for (auto& row : rows)
    {
        for (auto& node : row.columns)
        {
            if (!node.active || node.preview) 
            {
                continue; 
            }

            auto& data = nodeMesh->instanceData[node.instancedMeshIndex];
            data.world.r[0].m128_f32[0] = std::lerp(data.world.r[0].m128_f32[0], 0.9f, deltaTime * lerpSpeed);
            data.world.r[1].m128_f32[1] = std::lerp(data.world.r[1].m128_f32[1], 0.9f, deltaTime * lerpSpeed);
            data.world.r[2].m128_f32[2] = std::lerp(data.world.r[2].m128_f32[2], 0.9f, deltaTime * lerpSpeed);
        }
    }
}

void Grid::DisplayHideAllNodes()
{
    lerpValue = LerpValue::LerpIn;

    for (auto& row : rows)
    {
        for (auto& node : row.columns)
        {
            node.DisplayHide();
        }
    }
}

void Grid::DisplayShowAllNodes()
{
    lerpValue = LerpValue::LerpOut;

    for (auto& row : rows)
    {
        for (auto& node : row.columns)
        {
            node.DisplayShow();
        }
    }
}

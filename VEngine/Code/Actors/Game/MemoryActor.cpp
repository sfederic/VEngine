#include "MemoryActor.h"
#include "Gameplay/GameUtils.h"
#include "Render/Material.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Grid.h"
#include "Gameplay/GridNode.h"

MemoryActor::MemoryActor()
{
    isGridObstacle = isMemoryCreated;
}

void MemoryActor::Start()
{
    __super::Start();

    mesh->SetTexture("memory_pattern.png");
}

void MemoryActor::Tick(float deltaTime)
{
    __super::Tick(deltaTime);
}

Properties MemoryActor::GetProps()
{
    auto props = __super::GetProps();
    props.AddProp(memoryName);
    props.AddProp(isMemoryCreated);
    return props;
}

void MemoryActor::Interact()
{
    ActivateFromMemory();
}

void MemoryActor::ActivateFromMemory()
{
    if (!isMemoryCreated && GameUtils::CheckIfMemoryExists(memoryName))
    {
        //Stop metarial animation
        mesh->material->uvOffsetSpeed = XMFLOAT2(0.f, 0.f);
        mesh->material->uvRotationSpeed = 0.f;
        mesh->SetTexture(mesh->material->textureData.filename);

        GetCurrentNode()->Hide();

        GameUtils::PlayAudioOneShot("intuition_gained.wav");

        isGridObstacle = true;
        mesh->gridObstacle = true;

        //Reset world position of all grid nodes
        GameUtils::GetGrid()->Awake();

        isMemoryCreated = true;
    }
}

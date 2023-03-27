#include "vpch.h"
#include "MemoryActor.h"
#include "Gameplay/GameUtils.h"
#include "Render/Material.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/Grid.h"
#include "Core/Input.h"
#include "Gameplay/GridNode.h"

MemoryActor::MemoryActor()
{
    isGridObstacle = isMemoryCreated;

    trigger = BoxTriggerComponent::system.Add("Trigger", this);
    trigger->renderWireframeColour = XMFLOAT4(1.f, 0.f, 1.f, 1.f);
    trigger->SetTargetAsPlayer();
    rootComponent->AddChild(trigger);
}

void MemoryActor::Start()
{
    __super::Start();

    mesh->SetTexture("memory_pattern.png");
}

void MemoryActor::Tick(float deltaTime)
{
    __super::Tick(deltaTime);

    if (!isMemoryCreated && trigger->ContainsTarget())
    {
        if (Input::GetKeyUp(Keys::Down))
        {
            ActivateFromMemory();
        }
    }
}

Properties MemoryActor::GetProps()
{
    auto props = __super::GetProps();
    props.Add("Memory Name", &memoryName);
    props.Add("IsMemoryCreated", &isMemoryCreated);
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
        mesh->GetMaterial().uvOffsetSpeed = XMFLOAT2(0.f, 0.f);
        mesh->GetMaterial().uvRotationSpeed = 0.f;
        mesh->SetTexture(mesh->GetMaterial().textureData.filename);

        GetCurrentNode()->Hide();

        GameUtils::PlayAudioOneShot("intuition_gained.wav");

        isGridObstacle = true;

        //Reset world position of all grid nodes
        Grid::system.GetFirstActor()->Awake();

        isMemoryCreated = true;
    }
}

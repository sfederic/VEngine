#include "vpch.h"
#include "MemoryActor.h"
#include "Gameplay/GameUtils.h"
#include "Render/Material.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/Grid.h"
#include "Input.h"
#include "Gameplay/GridNode.h"

MemoryActor::MemoryActor()
{
    isGridObstacle = isMemoryCreated;

    trigger = BoxTriggerComponent::system.Add(this);
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

        //Reset world position of all grid nodes
        GameUtils::GetGrid()->Awake();

        isMemoryCreated = true;
    }
}

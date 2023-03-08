#include "vpch.h"
#include "Billboard.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"
#include "Core/Camera.h"
#include "Core/VMath.h"

Billboard::Billboard()
{
    mesh = MeshComponent::system.Add("Mesh", this,
        MeshComponent("plane.vmesh", "ramza_back.png", ShaderItems::Unlit));

    mesh->SetRastState(RastStates::noBackCull);
    rootComponent = mesh;
}

void Billboard::Tick(float deltaTime)
{
    auto rootTransform = rootComponent->GetTransform();
    VMath::RotateTowardsCamera(rootTransform);
}

Properties Billboard::GetProps()
{
    return Actor::GetProps();
}

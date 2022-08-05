#include "vpch.h"
#include "Billboard.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"
#include "Camera.h"
#include "VMath.h"

Billboard::Billboard()
{
    mesh = MeshComponent::system.Add("Mesh", this,
        MeshComponent("plane.fbx", "ramza_back.png", ShaderPairs::Unlit));

    mesh->SetRastState(RastStates::noBackCull);
    rootComponent = mesh;
}

void Billboard::Tick(float deltaTime)
{
    VMath::RotateTowardsCamera(rootComponent->transform);
}

Properties Billboard::GetProps()
{
    return Actor::GetProps();
}

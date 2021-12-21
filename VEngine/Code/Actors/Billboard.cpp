#include "Billboard.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"
#include "Camera.h"
#include "VMath.h"

Billboard::Billboard()
{
    mesh = MeshComponent::system.Add(this, MeshComponent("plane.fbx", "ramza_back.png", "Unlit.hlsl"));
    mesh->material->rastStateName = "nobackcull";
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

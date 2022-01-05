#include "FenceActor.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"

FenceActor::FenceActor()
{
    mesh = MeshComponent::system.Add(this, MeshComponent("plane.fbx", "test_fence.png"));
    rootComponent = mesh;
    mesh->material->rastStateName = "nobackcull";
}

Properties FenceActor::GetProps()
{
    return __super::GetProps();
}

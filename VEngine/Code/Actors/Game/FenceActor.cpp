#include "vpch.h"
#include "FenceActor.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"

FenceActor::FenceActor()
{
    mesh = MeshComponent::system.Add("Mesh", this, MeshComponent("plane.fbx", "test_fence.png"));
    rootComponent = mesh;
    mesh->material->rastStateValue.SetValue(RastStates::noBackCull);
}

Properties FenceActor::GetProps()
{
    return __super::GetProps();
}

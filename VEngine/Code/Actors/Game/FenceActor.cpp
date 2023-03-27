#include "vpch.h"
#include "FenceActor.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"
#include "Render/RastStates.h"

FenceActor::FenceActor()
{
    mesh = MeshComponent::system.Add("Mesh", this, MeshComponent("plane.vmesh", "test_fence.png"));
    rootComponent = mesh;
    mesh->GetMaterial().rastStateValue.SetValue(RastStates::noBackCull);
}

Properties FenceActor::GetProps()
{
    return __super::GetProps();
}

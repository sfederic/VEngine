#include "vpch.h"
#include "DestructibleActor.h"
#include "Components/DestructibleMeshComponent.h"

DestructibleActor::DestructibleActor()
{
    destructibleMesh = DestructibleMeshComponent::system.Add("Destructible", this,
        DestructibleMeshComponent("destructible_cube.vmesh", "test.png"));
    rootComponent = destructibleMesh;
}

void DestructibleActor::Start()
{
}

void DestructibleActor::Tick(float deltaTime)
{
}

Properties DestructibleActor::GetProps()
{
    auto props = __super::GetProps();
    return props;
}

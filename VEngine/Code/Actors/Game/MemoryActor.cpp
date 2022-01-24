#include "MemoryActor.h"
#include "Components/MeshComponent.h"

MemoryActor::MemoryActor()
{
    mesh = MeshComponent::system.Add(this);
    rootComponent = mesh;
}

void MemoryActor::Start()
{
}

void MemoryActor::Tick(float deltaTime)
{
}

Properties MemoryActor::GetProps()
{
    auto props = __super::GetProps();
    return props;
}

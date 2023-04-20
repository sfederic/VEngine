#include "vpch.h"
#include "MemoryActor.h"
#include "Components/MeshComponent.h"

void MemoryActor::Create()
{
    mesh->SetShaderItem(ShaderItems::MemoryObject);
}

Properties MemoryActor::GetProps()
{
    auto props = __super::GetProps();
    props.Add("Memory Name", &memoryName);
    return props;
}

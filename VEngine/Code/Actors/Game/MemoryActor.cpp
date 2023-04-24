#include "vpch.h"
#include "MemoryActor.h"
#include "Components/MeshComponent.h"
#include "Components/Game/MemoryComponent.h"

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

void MemoryActor::SetMemoryName(std::wstring_view memoryName)
{
    memoryComponent->memoryName = memoryName;
}

void MemoryActor::AddOnInteract(bool add)
{
    memoryComponent->addOnInteract = add;
}

#include "vpch.h"
#include "MemoryActor.h"
#include "Components/MeshComponent.h"
#include "Components/Game/MemoryComponent.h"
#include "Gameplay/MemorySystem.h"

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

bool MemoryActor::CheckMemory()
{
    return MemorySystem::Get().FindMemory(memoryName) != nullptr;
}

void MemoryActor::SetMemoryName(std::string_view memoryName)
{
    memoryComponent->memoryName = memoryName;
}

void MemoryActor::AddOnInteract(bool add)
{
    memoryComponent->addOnInteract = add;
}

#include "MemoryActor.h"
#include "Gameplay/GameUtils.h"

MemoryActor::MemoryActor()
{
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

void MemoryActor::Interact()
{
    if (GameUtils::CheckIfMemoryExists(memoryName))
    {
        //@Todo: change the texture if the memory exists, change some other grid node stuff
    }
}

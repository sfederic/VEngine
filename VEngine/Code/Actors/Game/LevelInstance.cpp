#include "vpch.h"
#include "LevelInstance.h"
#include "Components/BoxTriggerComponent.h"

LevelInstance::LevelInstance()
{
    boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
    boxTrigger->SetVisibility(false);
    rootComponent = boxTrigger;
}

Properties LevelInstance::GetProps()
{
    auto props = __super::GetProps();
    props.title = "LevelInstance";
    return props;
}

bool LevelInstance::CheckIfPointInsideLevelBounds(XMVECTOR point)
{
    return boxTrigger->Contains(point);
}

void LevelInstance::SetExtents(float x, float y, float z)
{
    boxTrigger->SetExtents(x, y, z);
}

XMVECTOR LevelInstance::GetRandomRoundedPointInsideLevelBounds()
{
    return boxTrigger->GetRandomPointInTrigger();
}

XMVECTOR LevelInstance::GetPositonOfFaceFromBounds(XMVECTOR boundsFaceDirection)
{
    auto triggerCenter = boxTrigger->GetWorldPositionV();
    auto extents = boxTrigger->GetBoundsExtents();
    auto result = triggerCenter + (boundsFaceDirection * extents);
    return result;
}

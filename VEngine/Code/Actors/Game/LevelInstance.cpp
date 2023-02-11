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

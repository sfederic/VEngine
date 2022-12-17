#include "vpch.h"
#include "LevelInstance.h"

LevelInstance::LevelInstance()
{
    SetEmptyRootComponent();
}

Properties LevelInstance::GetProps()
{
    auto props = __super::GetProps();
    props.Add("Level Size", &levelSize);
    return props;
}

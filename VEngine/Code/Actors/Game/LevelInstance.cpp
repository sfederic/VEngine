#include "vpch.h"
#include "LevelInstance.h"

LevelInstance::LevelInstance()
{
    SetEmptyRootComponent();
}

Properties LevelInstance::GetProps()
{
    auto props = __super::GetProps();
    props.Add("Level Size X", &levelSizeX);
    props.Add("Level Size Y", &levelSizeY);
    props.Add("Level Size Z", &levelSizeZ);
    return props;
}

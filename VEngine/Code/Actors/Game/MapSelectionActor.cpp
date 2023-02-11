#include "vpch.h"
#include "MapSelectionActor.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/MapScreenSelector.h"

MapSelectionActor::MapSelectionActor()
{
    boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
}

void MapSelectionActor::Tick(float deltaTime)
{
    auto mapScreenSelector = MapScreenSelector::system.GetFirstActor();
    if (mapScreenSelector == nullptr) return;

    if (boxTrigger->Contains(mapScreenSelector->GetPositionV()))
    {

    }
}

Properties MapSelectionActor::GetProps()
{
    auto props = __super::GetProps();
    props.title = GetTypeName();
    return props;
}

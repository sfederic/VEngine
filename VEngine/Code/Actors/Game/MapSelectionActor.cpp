#include "vpch.h"
#include "MapSelectionActor.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/MapScreenSelector.h"

MapSelectionActor::MapSelectionActor()
{
    boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
    rootComponent = boxTrigger;
}

void MapSelectionActor::Tick(float deltaTime)
{
    auto mapScreenSelector = MapScreenSelector::system.GetFirstActor();
    if (mapScreenSelector == nullptr) return;

    if (boxTrigger->Contains(mapScreenSelector->GetPositionV()))
    {
        mapScreenSelector->SetSelectionInfoWidgetFields(title, description, levelToLoad);
    }
    else
    {
        mapScreenSelector->RemoveMapSelectionInfoWidgetFromViewport();
    }
}

Properties MapSelectionActor::GetProps()
{
    auto props = __super::GetProps();
    props.title = GetTypeName();
    props.Add("Title", &title);
    props.Add("Desc.", &description);
    props.Add("Level", &levelToLoad);
    return props;
}

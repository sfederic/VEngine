#include "vpch.h"
#include "MapSelectionActor.h"
#include "Components/BoxTriggerComponent.h"
#include "Gameplay/GameInstance.h"
#include "Actors/Game/MapScreenPlayer.h"

MapSelectionActor::MapSelectionActor()
{
    boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
    rootComponent = boxTrigger;
}

void MapSelectionActor::Start()
{
    //Example code: how to activate this actor using global prop.
    /*auto value = GameInstance::GetGlobalProp<bool>("RamielDefeated");
    if (!value)
    {
        SetActive(false);
    }*/
}

void MapSelectionActor::Tick(float deltaTime)
{
    auto mapScreenPlayer = MapScreenPlayer::system.GetFirstActor();
    if (mapScreenPlayer == nullptr) return;

    if (boxTrigger->Contains(mapScreenPlayer->GetPositionV()))
    {
        mapScreenPlayer->SetLevelInfoWidgets(title, description, levelToLoad);
    }
    else
    {
        mapScreenPlayer->RemoveLevelInfoWidgets();
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

XMVECTOR MapSelectionActor::GetPlayerReturnSpawnPoint()
{
    auto pos = GetPosition();
    pos.y += boxTrigger->GetExtents().y + 2.f;
    return XMLoadFloat3(&pos);
}

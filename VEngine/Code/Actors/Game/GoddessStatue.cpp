#include "GoddessStatue.h"
#include "UI/PassTimeWidget.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameInstance.h"

GoddessStatue::GoddessStatue()
{
    mesh->meshComponentData.filename = "save_slate.fbx";
}

Properties GoddessStatue::GetProps()
{
    return __super::GetProps();
}

void GoddessStatue::Interact()
{
    auto widget = CreateWidget<PassTimeWidget>();
    widget->hoursToPass = GameInstance::currentHour;
    widget->AddToViewport();
}

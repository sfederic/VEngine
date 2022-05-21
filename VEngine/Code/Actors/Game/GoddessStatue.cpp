#include "vpch.h"
#include "GoddessStatue.h"
#include "UI/PassTimeWidget.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameInstance.h"

GoddessStatue::GoddessStatue()
{
    mesh->meshComponentData.filename = "save_slate.fbx";
}

void GoddessStatue::Start()
{
    passTimeWidget = CreateWidget<PassTimeWidget>();
}

Properties GoddessStatue::GetProps()
{
    return __super::GetProps();
}

void GoddessStatue::Interact()
{
    passTimeWidget->hoursToPass = GameInstance::currentHour;
    passTimeWidget->AddToViewport();
}

void GoddessStatue::EndInteract()
{
    passTimeWidget->RemoveFromViewport();
}

#include "GoddessStatue.h"
#include "UI/PassTimeWidget.h"
#include "Gameplay/GameInstance.h"

GoddessStatue::GoddessStatue()
{
}

Properties GoddessStatue::GetProps()
{
    return __super::GetProps();
}

void GoddessStatue::Interact()
{
    auto widget = CreateWidget<PassTimeWidget>();
    widget->tempHour = GameInstance::currentHour;
    widget->tempMinute = GameInstance::currentMinute;
    widget->AddToViewport();
}

#include "vpch.h"
#include "GoddessStatue.h"
#include "Components/MeshComponent.h"

GoddessStatue::GoddessStatue()
{
    mesh->meshComponentData.filename = "save_slate.fbx";
}

void GoddessStatue::Start()
{
}

Properties GoddessStatue::GetProps()
{
    return __super::GetProps();
}

void GoddessStatue::Interact()
{
}

void GoddessStatue::EndInteract()
{
}

#include "vpch.h"
#include "OilSlick.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameUtils.h"

void OilSlick::Create()
{
    canBeRotatedInLink = false;
    canBeRotatedInLink = false;

    mesh->SetMeshFilename("node.vmesh");
    mesh->SetTexture("oil_slick.png");
}

Properties OilSlick::GetProps()
{
    auto props = __super::GetProps();
    props.title = GetTypeName();
    return props;
}

void OilSlick::Burn()
{
    if (!onFire)
    {
        GameUtils::SpawnSpriteSheet("Sprites/fire_sheet.png", GetPositionV(), true, 4, 4);
        onFire = true;
    }
}

#include "vpch.h"
#include "RamielBoss.h"
#include "Actors/Game/Player.h"
#include "Components/MeshComponent.h"
#include "Physics/Raycast.h"
#include "Gameplay/GameUtils.h"

RamielBoss::RamielBoss()
{
    mesh = CreateComponent(MeshComponent("ramielboss.vmesh", ""), "Mesh");
    rootComponent = mesh;
}

void RamielBoss::Create()
{
    health = 100;

    mesh->SetUseTexture(false);
    mesh->SetAmbientColour(ambientColour);
}

void RamielBoss::Tick(float deltaTime)
{
    chargeAttackTimer += deltaTime;
    constexpr float chargeAttackMaxTime = 5.f;

    //Change colour over time as attack indicator
    mesh->SetAmbientColour(ambientColour);
    ambientColour.x += deltaTime / chargeAttackMaxTime;

    if (chargeAttackTimer > chargeAttackMaxTime)
    {
        FireChargeBeamAtPlayer();
        chargeAttackTimer = 0.f;
        ambientColour.x = 0.1f;
    }
}

Properties RamielBoss::GetProps()
{
    auto props = __super::GetProps();
    props.title = "RamielBoss";
    return props;
}

void RamielBoss::FireChargeBeamAtPlayer()
{
    HitResult hit(this);
    auto player = Player::system.GetFirstActor();
    XMVECTOR playerPos = player->GetPositionV();
    if (Raycast(hit, GetPositionV(), playerPos))
    {
        GameUtils::SpawnSpriteSheet("Sprites/blue_explosion.png", hit.GetHitPosV(), false, 5, 5);
    }
}

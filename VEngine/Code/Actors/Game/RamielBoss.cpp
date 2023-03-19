#include "vpch.h"
#include "RamielBoss.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/DestructibleObject.h"
#include "Components/MeshComponent.h"
#include "Physics/Raycast.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/GameInstance.h"
#include "UI/Game/BossHealthBar.h"
#include "UI/UISystem.h"

RamielBoss::RamielBoss()
{
    mesh = CreateComponent("Mesh", MeshComponent("ramielboss.vmesh", ""));
    rootComponent = mesh;
}

void RamielBoss::Create()
{
    health = 100;

    mesh->SetUseTexture(false);
    mesh->SetAmbientColour(ambientColour);
}

void RamielBoss::Start()
{
    bossHealthBar = UISystem::CreateWidget<BossHealthBar>();
    bossHealthBar->AddToViewport();
    bossHealthBar->SetBossName(L"Rammyel");
}

void RamielBoss::Tick(float deltaTime)
{
    bossHealthBar->SetBossHealth(health);

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

void RamielBoss::OnDestroyed()
{
    __super::OnDestroyed();

    GameInstance::SetGlobalProp("RamielDefeated", true);
}

void RamielBoss::FireChargeBeamAtPlayer()
{
    HitResult hit(this);
    auto player = Player::system.GetFirstActor();
    XMVECTOR playerPos = player->GetPositionV();
    if (Raycast(hit, GetPositionV(), playerPos))
    {
        GameUtils::SpawnSpriteSheet("Sprites/blue_explosion.png", hit.GetHitPosV(), false, 5, 5);

        auto destructibleObject = dynamic_cast<DestructibleObject*>(hit.hitActor);
        if (destructibleObject)
        {
            destructibleObject->Destroy();
        }
    }
}

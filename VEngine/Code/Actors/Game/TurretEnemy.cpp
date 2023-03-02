#include "vpch.h"
#include "TurretEnemy.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/TurretBullet.h"
#include "Gameplay/GameplayTags.h"
#include "Core/VMath.h"

void TurretEnemy::Create()
{
    __super::Create();

    auto mesh = CreateComponent("Mesh", MeshComponent("turret.vmesh", "test.png"));
    rootComponent->AddChild(mesh);
}

void TurretEnemy::Start()
{
    shootTimer = VMath::RandomRange(0.f, SHOOT_TIMER_MAX);
}

void TurretEnemy::Tick(float deltaTime)
{
    shootTimer += deltaTime;
    if (shootTimer > SHOOT_TIMER_MAX)
    {
        Shoot();
        shootTimer = 0.f;
    }
}

Properties TurretEnemy::GetProps()
{
    auto props = __super::GetProps();
    props.title = "TurretEnemy";
    return props;
}

void TurretEnemy::Shoot()
{
    auto transform = GetTransform();
    auto bullet = TurretBullet::system.Add(transform);
    bullet->SetMovementDirection(GetForwardVectorV());
}

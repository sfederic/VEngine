#include "vpch.h"
#include "TurretEnemy.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/TurretBullet.h"
#include "Gameplay/GameplayTags.h"

void TurretEnemy::Create()
{
    auto mesh = CreateComponent(MeshComponent("shield.fbx", "test.png"), "Mesh");
    rootComponent->AddChild(mesh);
}

Properties TurretEnemy::GetProps()
{
    auto props = __super::GetProps();
    props.title = "TurretEnemy";
    return props;
}

void TurretEnemy::Shoot()
{
    auto bullet = TurretBullet::system.Add(GetTransform());
    bullet->SetMovementDirection(GetForwardVectorV());
}

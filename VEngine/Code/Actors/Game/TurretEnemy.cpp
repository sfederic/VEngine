#include "vpch.h"
#include "TurretEnemy.h"
#include "Components/MeshComponent.h"
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
    auto forward = GetForwardVectorV();
    auto bulletMesh = MeshComponent::system.Add("Bullet", nullptr, MeshComponent("sphere.fbx", "test.png"));
}

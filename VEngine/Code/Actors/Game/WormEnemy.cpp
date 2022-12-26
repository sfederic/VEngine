#include "vpch.h"
#include "WormEnemy.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameplayTags.h"

void WormEnemy::Create()
{
    auto mesh1 = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh1");
    mesh1->SetScale(0.9f, 0.9f, 0.9f);
    mesh1->AddTag(GameplayTags::EnemyMeshPiece);
    rootComponent->AddChild(mesh1);

    auto mesh2 = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh2");
    mesh2->SetScale(0.9f, 0.9f, 0.9f);
    mesh2->SetPosition(0.f, 1.f, 0.f);
    mesh2->AddTag(GameplayTags::EnemyMeshPiece);
    rootComponent->AddChild(mesh2);

    auto mesh3 = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh3");
    mesh3->SetScale(0.9f, 0.9f, 0.9f);
    mesh3->SetPosition(0.f, 2.f, 0.f);
    mesh3->AddTag(GameplayTags::EnemyMeshPiece);
    rootComponent->AddChild(mesh3);
}

void WormEnemy::Tick(float deltaTime)
{
    constexpr float rotateSpeed = 30.f;

    auto mesh1 = GetComponentByNameAndType<MeshComponent>("Mesh1");
    if (mesh1)
    {
        mesh1->AddRotation(GetUpVectorV(), rotateSpeed * deltaTime);
    }

    auto mesh2 = GetComponentByNameAndType<MeshComponent>("Mesh2");
    if (mesh2)
    {
        mesh2->AddRotation(GetUpVectorV(), -rotateSpeed * deltaTime);
    }

    auto mesh3 = GetComponentByNameAndType<MeshComponent>("Mesh3");
    if (mesh3)
    {
        mesh3->AddRotation(GetUpVectorV(), rotateSpeed * deltaTime);
    }
}

Properties WormEnemy::GetProps()
{
    auto props = __super::GetProps();
    props.title = "WormEnemy";
    return props;
}

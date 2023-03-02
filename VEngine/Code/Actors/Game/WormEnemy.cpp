#include "vpch.h"
#include "WormEnemy.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameplayTags.h"

void WormEnemy::Create()
{
    __super::Create();

    auto mesh1 = CreateComponent("Mesh1", MeshComponent("cube.vmesh", "test.png"));
    mesh1->SetLocalScale(0.9f, 0.9f, 0.9f);
    mesh1->AddTag(GameplayTags::EnemyMeshPiece);
    rootComponent->AddChild(mesh1);

    auto mesh2 = CreateComponent("Mesh2", MeshComponent("cube.vmesh", "test.png"));
    mesh2->SetLocalScale(0.9f, 0.9f, 0.9f);
    mesh2->SetLocalPosition(0.f, 1.f, 0.f);
    mesh2->AddTag(GameplayTags::EnemyMeshPiece);
    rootComponent->AddChild(mesh2);

    auto mesh3 = CreateComponent("Mesh3", MeshComponent("cube.vmesh", "test.png"));
    mesh3->SetLocalScale(0.9f, 0.9f, 0.9f);
    mesh3->SetLocalPosition(0.f, 2.f, 0.f);
    mesh3->AddTag(GameplayTags::EnemyMeshPiece);
    rootComponent->AddChild(mesh3);
}

void WormEnemy::Tick(float deltaTime)
{
    constexpr float rotateSpeed = 30.f;

    auto mesh1 = GetComponent<MeshComponent>("Mesh1");
    if (mesh1)
    {
        mesh1->AddLocalRotation(GetUpVectorV(), rotateSpeed * deltaTime);
    }

    auto mesh2 = GetComponent<MeshComponent>("Mesh2");
    if (mesh2)
    {
        mesh2->AddLocalRotation(GetUpVectorV(), -rotateSpeed * deltaTime);
    }

    auto mesh3 = GetComponent<MeshComponent>("Mesh3");
    if (mesh3)
    {
        mesh3->AddLocalRotation(GetUpVectorV(), rotateSpeed * deltaTime);
    }
}

Properties WormEnemy::GetProps()
{
    auto props = __super::GetProps();
    props.title = "WormEnemy";
    return props;
}

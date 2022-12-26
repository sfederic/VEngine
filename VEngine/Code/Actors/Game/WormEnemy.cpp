#include "vpch.h"
#include "WormEnemy.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameplayTags.h"
#include "Particle/Polyboard.h"

void WormEnemy::Create()
{
    mesh1 = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh1");
    mesh1->SetScale(0.9f, 0.9f, 0.9f);
    mesh1->AddTag(GameplayTags::EnemyMeshPiece);
    rootComponent->AddChild(mesh1);

    mesh2 = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh2");
    mesh2->SetScale(0.9f, 0.9f, 0.9f);
    mesh2->SetPosition(0.f, 1.f, 0.f);
    mesh2->AddTag(GameplayTags::EnemyMeshPiece);
    rootComponent->AddChild(mesh2);

    mesh3 = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh3");
    mesh3->SetScale(0.9f, 0.9f, 0.9f);
    mesh3->SetPosition(0.f, 2.f, 0.f);
    mesh3->AddTag(GameplayTags::EnemyMeshPiece);
    rootComponent->AddChild(mesh3);

    beam = CreateComponent(Polyboard(), "Beam");
    rootComponent->AddChild(beam);
    beam->GenerateVertices();
}

void WormEnemy::Tick(float deltaTime)
{
    constexpr float rotateSpeed = 30.f;
    mesh1->AddRotation(GetUpVectorV(), rotateSpeed * deltaTime);
    mesh2->AddRotation(GetUpVectorV(), -rotateSpeed * deltaTime);
    mesh3->AddRotation(GetUpVectorV(), rotateSpeed * deltaTime);

    beam->startPoint = GetPosition();
    XMStoreFloat3(&beam->endPoint, mesh1->GetPositionV() + mesh1->GetForwardVectorV() * 10.f);
}

Properties WormEnemy::GetProps()
{
    auto props = __super::GetProps();
    props.title = "WormEnemy";
    return props;
}

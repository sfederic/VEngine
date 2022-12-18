#include "vpch.h"
#include "WormEnemy.h"
#include "Components/MeshComponent.h"

WormEnemy::WormEnemy()
{
    auto mesh1 = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh1");
    rootComponent->AddChild(mesh1);

    auto mesh2 = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh2");
    mesh2->SetPosition(0.f, 1.f, 0.f);
    rootComponent->AddChild(mesh2);
}

Properties WormEnemy::GetProps()
{
    auto props = __super::GetProps();
    props.title = "WormEnemy";
    return props;
}

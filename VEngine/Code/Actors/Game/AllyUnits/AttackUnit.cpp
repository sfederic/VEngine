#include "vpch.h"
#include "AttackUnit.h"
#include "Components/MeshComponent.h"

AttackUnit::AttackUnit()
{
    mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh");
    rootComponent->AddChild(mesh);
}

void AttackUnit::Start()
{
    __super::Start();
}

void AttackUnit::Tick(float deltaTime)
{
    __super::Tick(deltaTime);
}

Properties AttackUnit::GetProps()
{
    return __super::GetProps();
}

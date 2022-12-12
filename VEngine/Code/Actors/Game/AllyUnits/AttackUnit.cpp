#include "vpch.h"
#include "AttackUnit.h"
#include "Components/MeshComponent.h"

AttackUnit::AttackUnit()
{
    mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh");
    rootComponent->AddChild(mesh);
}

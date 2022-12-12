#include "vpch.h"
#include "SurroundAttackUnit.h"
#include "Components/MeshComponent.h"

SurroundAttackUnit::SurroundAttackUnit()
{
	mesh = CreateComponent(MeshComponent("char.fbx", "test.png"), "Mesh");
	rootComponent->AddChild(mesh);
}

void SurroundAttackUnit::AttackPattern()
{
}

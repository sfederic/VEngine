#include "vpch.h"
#include "AttackUnit.h"
#include "Components/MeshComponent.h"

AttackUnit::AttackUnit()
{
    mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh");
    rootComponent->AddChild(mesh);
}

std::vector<Unit*> AttackUnit::AttackPattern()
{
	std::vector<Unit*> hitUnits;
	const auto facingPosition = GetPositionV() + mesh->GetForwardVectorV();

	const int x = std::lroundf(facingPosition.m128_f32[0]);
	const int y = std::lroundf(facingPosition.m128_f32[2]);
	return hitUnits;
}

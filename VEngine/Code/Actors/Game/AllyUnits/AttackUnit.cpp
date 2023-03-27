#include "vpch.h"
#include "AttackUnit.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/Unit.h"

AttackUnit::AttackUnit()
{
    mesh = CreateComponent("Mesh", MeshComponent("cube.vmesh", "test.png"));
    rootComponent->AddChild(mesh);
}

void AttackUnit::AttackPattern()
{
	const auto facingPosition = GetPositionV() + mesh->GetForwardVectorV();
	const int x = std::lroundf(facingPosition.m128_f32[0]);
	const int y = std::lroundf(facingPosition.m128_f32[2]);

	auto unit = Grid::system.GetFirstActor()->GetUnitAtNodeIndex(x, y);
	if (unit)
	{
		unit->InflictDamage(attackPoints);
	}
}

#include "vpch.h"
#include "ShootAttackUnit.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/Unit.h"

ShootAttackUnit::ShootAttackUnit()
{
	mesh = CreateComponent("Mesh", MeshComponent("char.vmesh", "test.png"));
	rootComponent->AddChild(mesh);
}

void ShootAttackUnit::AttackPattern()
{
	auto forward = mesh->GetForwardVector();

	int xOffset = std::lroundf(forward.x);
	int yOffset = std::lroundf(forward.z);

	Unit* hitUnit = nullptr;

	int currentAttackX = xIndex;
	int currentAttackY = yIndex;

	const int shootRange = 5;
	for (int i = 0; i < shootRange; i++)
	{
		auto grid = Grid::system.GetFirstActor();

		currentAttackX += xOffset;
		currentAttackY += yOffset;

		hitUnit = grid->GetUnitAtNodeIndex(currentAttackX, currentAttackY);
		if (hitUnit)
		{
			hitUnit->InflictDamage(attackPoints);
			return;
		}
	}
}

#include "vpch.h"
#include "ShootAttackUnit.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/Unit.h"

ShootAttackUnit::ShootAttackUnit()
{
	mesh = CreateComponent(MeshComponent("char.fbx", "test.png"), "Mesh");
	rootComponent->AddChild(mesh);
}

void ShootAttackUnit::AttackPattern()
{
	auto forward = mesh->GetForwardVector();

	int xOffset = forward.x;
	int yOffset = forward.z;

	Unit* hitUnit = nullptr;

	const int shootRange = 5;
	for (int i = 0; i < shootRange; i++)
	{
		auto grid = Grid::system.GetFirstActor();

		int x = xIndex + xOffset;
		int y = yIndex + yOffset;

		hitUnit = grid->GetUnitAtNodeIndex(x, y);
		if (hitUnit)
		{
			hitUnit->InflictDamage(attackPoints);
			return;
		}
	}
}

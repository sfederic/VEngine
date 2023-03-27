#include "vpch.h"
#include "ExplodingObject.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/Unit.h"

//Explode in a cross pattern.
void ExplodingObject::Attacked()
{
	GameUtils::SpawnSpriteSheet("Sprites/explosion.png", GetPositionV(), false, 4, 4);

	auto grid = Grid::system.GetFirstActor();

	auto explodingNodes = grid->GetNeighbouringActiveAndInactiveNodesForceful(GetCurrentNode());

	for (auto node : explodingNodes)
	{
		GameUtils::SpawnSpriteSheet("Sprites/explosion.png", node->GetWorldPosV(), false, 4, 4);

		auto unit = grid->GetUnitAtNode(node);
		if (unit)
		{
			unit->InflictDamage(1);
		}
	}
}

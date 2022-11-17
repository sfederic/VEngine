#include "vpch.h"
#include "ExplodingObject.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/Grid.h"
#include "Actors/Game/Unit.h"

//Explode in a cross pattern.
void ExplodingObject::Attacked()
{
	GameUtils::SpawnSpriteSheet("Sprites/explosion.png", GetPosition(), false, 4, 4);

	auto grid = Grid::system.GetFirstActor();

	std::vector<GridNode*> explodingNodes;
	grid->GetNeighbouringActiveAndInactiveNodesForceful(GetCurrentNode(), explodingNodes);

	for (auto node : explodingNodes)
	{
		GameUtils::SpawnSpriteSheet("Sprites/explosion.png", node->worldPosition, false, 4, 4);

		auto unit = grid->GetUnitAtNode(node);
		if (unit)
		{
			unit->InflictDamage(1);
		}
	}
}


#include "MapProjectionCrystal.h"
#include "MapProjectionActor.h"
#include "Grid.h"
#include "Physics/HitResult.h"

void MapProjectionCrystal::Interact()
{
	__super::Interact();

	for (auto& actor : MapProjectionActor::system.GetActors())
	{
		actor->ToggleActive();
	}

	auto grid = Grid::system.GetOnlyActor();
	HitResult hit(grid);
	grid->RecalcAllNodes(hit, true);
}

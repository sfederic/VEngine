#include "vpch.h"
#include "MapProjectionCrystal.h"
#include "MapProjectionActor.h"

void MapProjectionCrystal::Interact()
{
	__super::Interact();

	for (auto& actor : MapProjectionActor::system.GetActors())
	{
		actor->ToggleActive();
	}
}

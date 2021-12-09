#include "PickupSpawnData.h"
#include "Actors/Game/Pickup.h"
#include "Components/MeshComponent.h"

PickupSpawnData::PickupSpawnData()
{
}

PickupSpawnData::PickupSpawnData(Pickup* pickup)
{
	meshFilename = pickup->mesh->meshComponentData.filename;
	pickupName = pickup->pickupName;
}

#include "vpch.h"
#include "Hoist.h"
#include "VMath.h"

Hoist::Hoist()
{
}

void Hoist::Attacked()
{
	Actor* hoistedActor = World::GetActorByNameAllowNull(hoistedActorName);
	if (hoistedActor == nullptr)
	{
		Log("Actor [%s] linked to Hoist [%s] not found.", hoistedActorName.c_str(), GetName().c_str());
		return;
	}

	auto gridActor = dynamic_cast<GridActor*>(hoistedActor);
	if (gridActor)
	{
		gridActor->Pushback(-VMath::GlobalUpVector());
	}
}

#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class MeshComponent;

//Actors that are turned on and off via MapProjectionCrystals across the entire map.
//(Note that this isn't a GridActor. Don't want crazy stuff going with moving actors that can be toggled visually.)
class MapProjectionActor : public Actor
{
public:
	ACTOR_SYSTEM(MapProjectionActor);

	MapProjectionActor();
	void Create() override;
	Properties GetProps() override;

private:
	MeshComponent* _mesh = nullptr;
};

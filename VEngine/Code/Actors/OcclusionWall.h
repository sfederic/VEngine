#pragma once

#include "Actor.h"

class MeshComponent;

//An actor whose visibility will toggle based on a boxcast between the camera and player.
//Taken from how Vagrant Story handles its walls in maps based on camera orientation.
class OcclusionWall : public Actor
{
public:
	ACTOR_SYSTEM(OcclusionWall);

	OcclusionWall();
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	MeshComponent* mesh = nullptr;
};

#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;
class MeshComponent;

//Put a BloomSeed in this actor's trigger to plant it.
class BloomSeedSoil : public Actor
{
public:
	ACTOR_SYSTEM(BloomSeedSoil);
	BloomSeedSoil();
	void Create() override;
	void Tick(float deltaTime) override;
	DEFAULT_PROPS;

private:
	BoxTriggerComponent* boxTrigger = nullptr;
	MeshComponent* moundMesh = nullptr;
};

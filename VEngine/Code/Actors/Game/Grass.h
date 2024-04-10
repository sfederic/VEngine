#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class MeshComponent;

class Grass : public Actor
{
public:
	ACTOR_SYSTEM(Grass);

	Grass();
	void Create() override;
	Properties GetProps() override;

private:
	MeshComponent* mesh = nullptr;
};

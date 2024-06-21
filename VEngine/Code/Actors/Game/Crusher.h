#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class MeshComponent;

//Think like that Attack of the Clones scene in the factory.
class Crusher : public Actor
{
public:
	ACTOR_SYSTEM(Crusher);

	Crusher();
	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	MeshComponent* mesh = nullptr;
};

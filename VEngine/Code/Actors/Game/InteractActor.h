#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;

//Base class for an Actor that fires off an effect on Player interaction.
class InteractActor : public Actor
{
public:
	ACTOR_SYSTEM(InteractActor);

	InteractActor();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	virtual void Interact();

private:
	MeshComponent* mesh = nullptr;
};

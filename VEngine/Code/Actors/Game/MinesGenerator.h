#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;
class MeshComponent;

//Actor 'key' (e.g. activate 4 generators) in the mines that sets a global prop when turned on.
//Have to slot a crystal in to activate it.
class MinesGenerator : public Actor
{
public:
	ACTOR_SYSTEM(MinesGenerator);

	MinesGenerator();
	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void PowerOnGenerator();

	BoxTriggerComponent* crystalTrigger = nullptr;
	MeshComponent* mesh = nullptr;

	bool isGeneratorOn = false;
};

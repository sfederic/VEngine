#pragma once
#include "GridActor.h"

//Actor that is transparent in the world, giving a hint to the player to instantiate it
//by interacting with it with the appropriate memory held by the player.
struct MemoryActor : GridActor
{
	ACTOR_SYSTEM(MemoryActor);

	//The memory to instantiate the object
	std::string memoryName;

	//Whether this actor has already been created via player's memory
	bool isMemoryCreated = false;

	MemoryActor();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	virtual void Interact() override;

private:
	void ActivateFromMemory();
};

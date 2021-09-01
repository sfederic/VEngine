#pragma once
#include <string>
#include "Serialiser.h"
#include "Transform.h"

struct Actor;

struct IActorSystem
{
	std::string name;

	virtual void Tick(double deltaTime) = 0;
	virtual void GetActors(std::vector<Actor*>& outActors) = 0;
	virtual Actor* SpawnActor(Transform transform) = 0;
	virtual Actor* FindActorByName(std::string actorName) = 0;
	virtual void Serialise(Serialiser& s) = 0;
	virtual void Deserialise(Serialiser& s) = 0;
	virtual void Cleanup() = 0;
};

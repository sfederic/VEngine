#pragma once
#include <vector>
#include <string>
#include <ostream>
#include <istream>
#include "Transform.h"

struct Actor;

struct IActorSystem
{
	std::string name;
	std::string actorTemplateFilename;

	virtual void Tick(double deltaTime) = 0;
	virtual void GetActors(std::vector<Actor*>& outActors) = 0;
	virtual Actor* SpawnActor(Transform transform) = 0;
	virtual Actor* SpawnActorTemplate() = 0;
	virtual Actor* FindActorByName(std::string actorName) = 0;
	virtual void Serialise(std::ostream& os) = 0;
	virtual void SerialiseActorTemplate(std::ostream& is, Actor* actor) = 0;
	virtual void Deserialise(std::istream& is) = 0;
	virtual Actor* DeserialiseActorTemplate(std::istream& is) = 0;
	virtual void Cleanup() = 0;
};

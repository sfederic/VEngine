#pragma once
#include <string>
#include "Transform.h"

struct Actor;
struct Serialiser;
struct BinarySerialiser;
struct Deserialiser;
struct BinaryDeserialiser;

class IActorSystem
{
protected:
	std::string name;

public:
	virtual void Tick(float deltaTime) = 0;
	virtual void Init() = 0;
	std::string GetName() { return name; }
	virtual std::vector<Actor*> GetActorsAsBaseClass() = 0;
	virtual Actor* SpawnActor(Transform transform) = 0;
	virtual Actor* FindActorByName(std::string actorName) = 0;
	virtual uint32_t GetNumActors() = 0;
	virtual void CreateAllActorComponents() = 0;
	virtual void Serialise(Serialiser& s) = 0;
	virtual void SerialiseBinary(BinarySerialiser& s) = 0;
	virtual void Deserialise(Deserialiser& s) = 0;
	virtual void DeserialiseBinary(BinaryDeserialiser& s) = 0;
	virtual void Cleanup() = 0;
};

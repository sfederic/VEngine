#pragma once

#include <string>

class Actor;
class Serialiser;
class BinarySerialiser;
class Deserialiser;
class BinaryDeserialiser;
struct Transform;

class IActorSystem
{
public:
	virtual void Tick(float deltaTime) = 0;
	virtual void Init() = 0;
	virtual void PostInit() = 0;
	std::string GetName() { return name; }
	virtual std::vector<Actor*> GetActorsAsBaseClass() = 0;
	virtual Actor* SpawnActor(const Transform& transform) = 0;
	virtual Actor* FindActorByName(std::string actorName) = 0;
	virtual uint32_t GetNumActors() = 0;

	//Destroys an actor through its linked ActorSystem when its base class does not ACTOR_SYSTEM() defined.
	virtual void RemoveInterfaceActor(Actor* actor) = 0;

	virtual void CreateAllActorComponents() = 0;
	virtual void Serialise(Serialiser& s) = 0;
	virtual void SerialiseBinary(BinarySerialiser& s) = 0;
	virtual void Deserialise(Deserialiser& s) = 0;
	virtual void DeserialiseBinary(BinaryDeserialiser& s) = 0;
	virtual void Cleanup() = 0;

protected:
	std::string name;
};

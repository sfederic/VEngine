#pragma once

#include "Core/SystemStates.h"
#include <string>

class Component;
class Actor;
class Serialiser;
class BinarySerialiser;
class Deserialiser;
class BinaryDeserialiser;

class IComponentSystem
{
public:
	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void Start() = 0;
	virtual void Cleanup() = 0;
	virtual void Serialise(Serialiser& s) = 0;
	virtual void SerialiseBinary(BinarySerialiser& s) = 0;
	virtual void Deserialise(Deserialiser& s) = 0;
	virtual void DeserialiseBinary(BinaryDeserialiser& d) = 0;
	virtual Component* SpawnComponent(Actor* owner) = 0;
	virtual std::vector<Component*> GetComponentsAsBaseClass() = 0;
	virtual uint32_t GetNumComponents() = 0;
	virtual Component* FindComponentByName(std::string componentName) = 0;

	auto GetName() { return name; }

protected:
	SystemStates systemState = SystemStates::Unloaded;
	std::string name;
};

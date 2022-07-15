#pragma once
#include "SystemStates.h"
#include <string>

struct Component;
struct Actor;
struct Serialiser;
struct Deserialiser;

struct IComponentSystem
{
	SystemStates systemState = SystemStates::Unloaded;

	std::string name;

	virtual void Init() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void Start() = 0;
	virtual void Cleanup() = 0;
	virtual void Serialise(Serialiser& s) = 0;
	virtual void Deserialise(Deserialiser& s) = 0;
	virtual Component* SpawnComponent(Actor* owner) = 0;
	virtual std::vector<Component*> GetComponents() = 0;
	virtual uint32_t GetNumComponents() = 0;
	virtual Component* FindComponentByName(std::string componentName) = 0;
};

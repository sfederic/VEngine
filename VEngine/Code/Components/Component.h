#pragma once
#include "Properties.h"
#include "UID.h"

struct IComponentSystem;

struct Component
{
	IComponentSystem* componentSystem = nullptr;
	UID uid = GenerateUID();
	std::string name;
	UID ownerUID = 0;
	int index = -1;
	bool active = true;

	virtual void Tick(float deltaTime) {}
	virtual void Start() {}
	virtual void Create() {};
	
	//Cleanup all the innards of the component.
	virtual void Destroy() {}

	//Remove the component from its parent ComponentSystem. Remove() is always defined in 
	//COMPONENT_SYSTEM macro and doesn't need to be added explicity.
	virtual void Remove() = 0;

	virtual Properties GetProps();
};

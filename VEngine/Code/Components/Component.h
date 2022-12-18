#pragma once

#include "Properties.h"
#include "UID.h"
#include <set>

struct IComponentSystem;

struct Component
{
	IComponentSystem* componentSystem = nullptr;
	UID uid = GenerateUID();
	std::string name;
	UID ownerUID = 0;
	int index = -1;
	bool active = true;
	bool tickEnabled = true;

	virtual void Tick(float deltaTime) {}
	virtual void Start() {}
	virtual void Create() {};
	
	//Cleanup all the innards of the component.
	virtual void Destroy() {}

	//Remove the component from its parent ComponentSystem. Remove() is always defined in 
	//COMPONENT_SYSTEM macro and doesn't need to be added explicity.
	virtual void Remove() = 0;

	virtual Properties GetProps()
	{
		Properties props{};

		//Because the serialise file format changed for components, had to put the Name and OwnerUID
		//separate from the other props so that components can find their owners on spawn (when defined in c++).
		//Leaving this here just for rough clarity.

		//props.Add("Name", &name).hide = true;
		//props.Add("OwnerUID", &ownerUID).hide = true;
		props.Add(" Enabled", &active);

		return props;
	}

	void AddTag(const std::string& tag);
	bool HasTag(const std::string& tag);

private:
	std::set<std::string> tags;
};

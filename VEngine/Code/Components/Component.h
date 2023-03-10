#pragma once

#include "Core/Properties.h"
#include "Core/UID.h"
#include <set>

class IComponentSystem;
class Actor;

class Component
{
public:
	virtual void Tick(float deltaTime) {}
	virtual void Start() {}
	virtual void Create() {};

	//Remove the component from its parent ComponentSystem. Remove() is always defined in 
	//COMPONENT_SYSTEM macro and doesn't need to be added explicity.
	virtual void Remove() = 0;

	virtual Properties GetProps()
	{
		Properties props;

		props.ownerUID = GetOwnerUID();

		//Because the serialise file format changed for components, had to put the Name and OwnerUID
		//separate from the other props so that components can find their owners on spawn (when defined in c++).
		//Leaving this here just for rough clarity to what it used to be.

		//props.Add("Name", &name).hide = true;
		//props.Add("OwnerUID", &ownerUID).hide = true;
		props.Add(" Enabled", &active);
		props.Add(" Visible", &visible);
		props.Add("UID", &uid).hide = true;

		return props;
	}

	//Returns pruned typeid() name from linked Component System.
	std::string GetTypeName();

	void AddTag(const std::string& tag);
	bool HasTag(const std::string& tag);

	bool IsActive() { return active; }
	void SetActive(bool newActive) { active = newActive; }

	int GetIndex() { return index; }
	void SetIndex(int newIndex) { index = newIndex; }

	void SetComponentSystem(IComponentSystem* componentSystem_) { componentSystem = componentSystem_; }

	UID GetUID() { return uid; }
	void SetUID(UID uid_) { uid = uid_; }

	UID GetOwnerUID() { return ownerUID; }
	void SetOwnerUID(UID ownerUID_) { ownerUID = ownerUID_; }

	Actor* GetOwner();

	bool IsTickEnabled() { return tickEnabled; }
	void SetTickEnabled(bool newTickState) { tickEnabled = newTickState; }

	bool IsVisible() { return visible; }
	void SetVisibility(bool isVisible) { visible = isVisible; }

protected:
	//Cleanup all the innards of the component.
	virtual void Destroy() {}

public:
	std::string name;

private:
	std::set<std::string> tags;
	IComponentSystem* componentSystem = nullptr;
	UID uid = GenerateUID();
	UID ownerUID = 0; //Keep as zero to denote component that doesn't have an owner.
	int index = -1;
	bool active = true;
	bool visible = true;
	bool tickEnabled = true;
};

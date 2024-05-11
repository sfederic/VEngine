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
	//COMPONENT_SYSTEM macro and doesn't need to be added explicitly.
	virtual void Remove() = 0;

	virtual Properties GetProps();

	//Returns pruned typeid() name from linked Component System.
	std::string GetTypeName();

	void AddTag(const std::string& tag);
	bool HasTag(const std::string& tag);

	bool IsActive() const { return active; }
	void SetActive(bool newActive) { active = newActive; }
	void ToggleActive() { active = !active; }

	auto GetIndex() const { return index; }
	void SetIndex(size_t newIndex) { index = newIndex; }

	void SetComponentSystem(IComponentSystem* componentSystem_) { componentSystem = componentSystem_; }

	UID GetUID() const { return uid; }
	void SetUID(UID uid_) { uid = uid_; }

	UID GetOwnerUID() const { return ownerUID; }
	void SetOwnerUID(UID ownerUID_) { ownerUID = ownerUID_; }

	Actor* GetOwner();

	bool IsTickEnabled() const { return tickEnabled; }
	void SetTickEnabled(bool newTickState) { tickEnabled = newTickState; }

	bool IsVisible() const { return visible; }
	void SetVisibility(bool isVisible) { visible = isVisible; }
	void ToggleVisibility() { visible = !visible; }

	//Cleanup all the innards of the component.
	virtual void Destroy() {}

	std::string name;

private:
	std::set<std::string> tags;
	IComponentSystem* componentSystem = nullptr;
	size_t index = 0;
	UID uid = 0;
	UID ownerUID = 0; //Keep as zero to denote component that doesn't have an owner.
	bool active = true;
	bool visible = true;
	bool tickEnabled = true;
};

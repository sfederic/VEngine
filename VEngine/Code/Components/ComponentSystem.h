#pragma once
#include <vector>
#include "IComponentSystem.h"
#include "SystemStates.h"
#include "Actors/Actor.h"

//TODO: I need to do a thing where if a component is created past the initial world load dynamically, defer
//that component's creation to the start of the next frame because of assigning values to its member variables.
//Maybe have two sepereate arrays (loaded vs unloaded components) and create new components at the next check.

template <typename T>
struct ComponentSystem : IComponentSystem
{
	std::vector<T*> components;

	T* Add(Actor* owner = nullptr, T newComponent = T())
	{
		components.push_back(new T(newComponent));

		T* component = components.back();
		component->owner = owner;
		if (systemState == SystemStates::Loaded)
		{
			component->Create();
		}

		owner->components.push_back(component);

		return component;
	}

	void Init()
	{
		for (T* component : components)
		{
			component->Create();
		}

		systemState = SystemStates::Loaded;
	}
};

#define COMPONENT_SYSTEM(type) \
inline static ComponentSystem<type> system; \

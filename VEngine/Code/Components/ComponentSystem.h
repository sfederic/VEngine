#pragma once
#include <vector>
#include "IComponentSystem.h"
#include "SystemStates.h"
#include "Actors/Actor.h"

template <typename T>
struct ComponentSystem : IComponentSystem
{
	std::vector<T*> components;

	T* Add(Actor* owner)
	{
		components.push_back(new T());

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

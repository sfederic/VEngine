#pragma once
#include <vector>
#include "IComponentSystem.h"
#include "SystemStates.h"
#include "Actors/Actor.h"

template <typename T>
struct ComponentSystem : IComponentSystem
{
	std::vector<T> components;

	SystemStates systemState = SystemStates::Unloaded;

	T* Add(Actor* owner)
	{
		components.push_back(T());

		T& component = components.back();
		component.owner = owner;
		if (systemState == SystemStates::Unloaded)
		{
			component.Create();
		}

		owner->components.push_back(&component);

		return &components.back();
	}

	void Init()
	{
		for (T& component : components)
		{
			component.Create();
		}

		systemState = SystemStates::Loaded;
	}
};

#define COMPONENT_SYSTEM(type) \
inline static ComponentSystem<type> system; \

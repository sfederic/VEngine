#pragma once
#include <vector>
#include "IComponentSystem.h"
#include "SystemStates.h"

template <typename T>
struct ComponentSystem : IComponentSystem
{
	std::vector<T> components;

	SystemStates systemState = SystemStates::Unloaded;

	T* Add()
	{
		components.push_back(T());

		T& component = components.back();
		if (systemState == SystemStates::Unloaded)
		{
			component.Create();
		}

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

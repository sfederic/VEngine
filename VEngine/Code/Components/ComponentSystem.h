#pragma once
#include <vector>
#include "IComponentSystem.h"

template <typename T>
struct ComponentSystem : IComponentSystem
{
	std::vector<T> components;

	T* Add()
	{
		components.push_back(T());
		return &components.back();
	}

	void Init()
	{
		for (T& component : components)
		{
			component.Create();
		}
	}
};

#define COMPONENT_SYSTEM(type) \
inline static ComponentSystem<type> system; \


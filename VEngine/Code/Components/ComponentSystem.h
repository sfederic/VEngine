#pragma once
#include <vector>
#include "IComponentSystem.h"
#include "SystemStates.h"
#include "Actors/Actor.h"
#include "ComponentSystemCache.h"

template <typename T>
struct ComponentSystem : IComponentSystem
{
	std::vector<T*> components;

	ComponentSystem()
	{
		std::string typeName = typeid(T).name();
		std::string delimiter = " ";
		std::string token = typeName.substr(typeName.find(delimiter) + 1);
		name = token;

		componentSystemCache.Add(typeid(T), this);
	}

	T* Add(Actor* owner = nullptr, T newComponent = T())
	{
		components.emplace_back(new T(newComponent));

		T* component = components.back();
		component->owner = owner;
		component->index = components.size() - 1;
		component->name = name + std::to_string(component->index);

		if (systemState == SystemStates::Loaded)
		{
			component->Create();
		}

		if (owner)
		{
			owner->components.push_back(component);
		}

		return component;
	}

	void Remove(int index)
	{
		delete components[index];
		std::swap(components[index], components.back());
		components[index]->index = index;
		components.pop_back();
	}

	void Init()
	{
		for (T* component : components)
		{
			component->Create();
		}

		systemState = SystemStates::Loaded;
	}

	virtual void Tick(double deltaTime) override
	{
		for (T* component : components)
		{
			component->Tick(deltaTime);
		}
	}

	virtual void Cleanup() override
	{
		for (T* component : components)
		{
			delete component;
		}

		components.clear();
	}

	virtual void GetComponents(std::vector<Component*>& outComponents) override
	{
		for (T* component : components)
		{
			outComponents.push_back(component);
		}
	}
};

#define COMPONENT_SYSTEM(type) \
inline static ComponentSystem<type> system; \
virtual void Destroy() override { system.Remove(index); } \

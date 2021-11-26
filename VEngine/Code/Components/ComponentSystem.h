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
		std::string token = typeName.substr(typeName.find(" ") + 1);
		name = token;

		componentSystemCache.Add(typeid(T), this);
	}

	T* Add(Actor* owner = nullptr, T newComponent = T())
	{
		T* component = new T(std::move(newComponent));
		components.emplace_back(component);

		component->owner = owner;
		component->index = components.size() - 1;
		component->name = this->name + std::to_string(component->index);

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
		std::swap(components[index], components.back());
		components[index]->index = index;
		components[index]->name = this->name + std::to_string(index);
		delete components.back();
		components.pop_back();
	}

	virtual void Init() override
	{
		for (T* component : components)
		{
			component->Create();
		}

		systemState = SystemStates::Loaded;
	}

	virtual void Start() override
	{
		for (T* component : components)
		{
			component->Start();
		}
	}

	virtual void Tick(float deltaTime) override
	{
		for (T* component : components)
		{
			if (component->active)
			{
				component->Tick(deltaTime);
			}
		}
	}

	virtual void Cleanup() override
	{
		for (T* component : components)
		{
			delete component;
		}

		components.clear();

		systemState = SystemStates::Unloaded;
	}

	virtual std::vector<Component*> GetComponents() override
	{
		std::vector<Component*> outComponents;

		for (T* component : components)
		{
			outComponents.push_back(component);
		}

		return outComponents;
	}

	uint32_t GetNumComponents() override
	{
		return components.size();
	}

	virtual Component* FindComponentByName(std::string componentName) override
	{
		for (T* component : components)
		{
			if (component->name == componentName)
			{
				return (Component*)component;
			}
		}

		return nullptr;
	}
};

#define COMPONENT_SYSTEM(type) \
inline static ComponentSystem<type> system; \
virtual void Remove() override { Destroy(); system.Remove(index); } \

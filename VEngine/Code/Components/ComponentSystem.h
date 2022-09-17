#pragma once
#include <vector>
#include "IComponentSystem.h"
#include "SystemStates.h"
#include "Actors/Actor.h"
#include "ComponentSystemCache.h"
#include "Serialiser.h"
#include "VString.h"
#include "World.h"

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

	T* Add(std::string name, Actor* owner = nullptr, T newComponent = T(), bool callCreate = true)
	{
		T* component = new T(std::move(newComponent));
		components.emplace_back(component);

		component->index = components.size() - 1;
		component->componentSystem = this;
		component->name = name;

		if (systemState == SystemStates::Loaded && callCreate)
		{
			component->Create();
		}

		if (owner)
		{
			owner->AddComponent(component);
		}

		return component;
	}

	virtual Component* SpawnComponent(Actor* owner) override
	{
		return (Component*)Add("", owner);
	}

	void Remove(int index)
	{
		std::swap(components[index], components.back());
		components[index]->index = index;
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
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i]->active && components[i]->tickEnabled)
			{
				components[i]->Tick(deltaTime);
			}
		}
	}

	virtual void Serialise(Serialiser& s) override
	{
		s.WriteLine(VString::stows(name));
		s.WriteLine(components.size());

		for (T* component : components)
		{
			s.WriteLine(component->ownerUID);
			s.WriteLine(VString::stows(component->name));

			auto props = component->GetProps();
			s.Serialise(props);

			s.WriteLine(L"next");
		}
	}

	virtual void SerialiseBinary(BinarySerialiser& s) override
	{
		s.WriteString(name);
		size_t numComponents = components.size();
		s.Write(&numComponents);

		for (T* component : components)
		{
			UID ownerUID = component->ownerUID;
			s.Write(&ownerUID);
			s.WriteString(component->name);

			auto props = component->GetProps();
			s.Serialise(props);
		}
	}

	virtual void Deserialise(Deserialiser& d) override
	{
		for (T* component : components)
		{
			auto props = component->GetProps();
			d.Deserialise(props);
		}
	}

	virtual void DeserialiseBinary(BinaryDeserialiser& d) override
	{
		for (T* component : components)
		{
			auto props = component->GetProps();
			d.Deserialise(props);
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

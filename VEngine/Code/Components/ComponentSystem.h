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
class ComponentSystem : public IComponentSystem
{
public:
	ComponentSystem()
	{
		std::string typeName = typeid(T).name();
		std::string token = typeName.substr(typeName.find(" ") + 1);
		name = token;

		componentSystemCache.Add(typeid(T), this);
	}

	T* Add(std::string name, Actor* owner = nullptr, T newComponent = T(), bool callCreate = true)
	{
		components.emplace_back(std::make_unique<T>(std::move(newComponent)));
		auto& component = components.back();

		component->index = components.size() - 1;
		component->componentSystem = this;
		component->name = name;

		if (systemState == SystemStates::Loaded && callCreate)
		{
			component->Create();
		}

		if (owner)
		{
			owner->AddComponent(component.get());
		}

		return component.get();
	}

	std::vector<std::unique_ptr<T>>& GetComponents()
	{
		return components;
	}

	virtual Component* SpawnComponent(Actor* owner) override
	{
		return (Component*)Add("", owner);
	}

	void Remove(int index)
	{
		std::swap(components[index], components.back());
		components[index]->index = index;
		components.pop_back();
	}

	virtual void Init() override
	{
		for (auto& component : components)
		{
			component->Create();
		}

		systemState = SystemStates::Loaded;
	}

	virtual void Start() override
	{
		for (auto& component : components)
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

	T* GetFirstComponent()
	{
		return components.front().get();
	}

	virtual void Serialise(Serialiser& s) override
	{
		s.WriteLine(VString::stows(name));
		s.WriteLine(components.size());

		for (auto& component : components)
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

		for (auto& component : components)
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
		for (auto& component : components)
		{
			auto props = component->GetProps();
			d.Deserialise(props);
		}
	}

	virtual void DeserialiseBinary(BinaryDeserialiser& d) override
	{
		for (auto& component : components)
		{
			auto props = component->GetProps();
			d.Deserialise(props);
		}
	}

	virtual void Cleanup() override
	{
		components.clear();
		systemState = SystemStates::Unloaded;
	}

	virtual std::vector<Component*> GetComponentsAsBaseClass() override
	{
		std::vector<Component*> outComponents;
		for (auto& component : components)
		{
			outComponents.push_back(component.get());
		}
		return outComponents;
	}

	uint32_t GetNumComponents() override
	{
		return components.size();
	}

	virtual Component* FindComponentByName(std::string componentName) override
	{
		for (auto& component : components)
		{
			if (component->name == componentName)
			{
				return (Component*)component.get();
			}
		}

		return nullptr;
	}

private:
	std::vector<std::unique_ptr<T>> components;
};

#define COMPONENT_SYSTEM(type) \
inline static ComponentSystem<type> system; \
virtual void Remove() override { Destroy(); system.Remove(index); } \

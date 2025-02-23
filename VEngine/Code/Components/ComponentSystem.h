#pragma once

#include <vector>
#include "IComponentSystem.h"
#include "Core/SystemStates.h"
#include "Actors/Actor.h"
#include "ComponentSystemCache.h"
#include "Core/Serialiser.h"
#include "Core/VString.h"
#include "Editor/Editor.h"
#include "Core/World.h"

template <typename T>
class ComponentSystem : public IComponentSystem
{
public:
	ComponentSystem()
	{
		std::string typeName = typeid(T).name();
		std::string token = typeName.substr(typeName.find(" ") + 1);
		_name = token;

		ComponentSystemCache::Get().Add(typeid(T), this);
	}

	T* Add(std::string name, Actor* owner = nullptr, T newComponent = T(), bool callCreate = false)
	{
		components.emplace_back(std::make_unique<T>(std::move(newComponent)));
		auto& component = components.back();

		component->SetIndex(components.size() - 1);
		component->SetComponentSystem(this);
		component->SetName(name);
		component->SetUID(GenerateUID());

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

	void Remove(size_t index)
	{
		std::swap(components[index], components.back());
		components[index]->SetIndex(index);

		components.back().get()->Destroy();

		if (components.back()->GetOwnerUID() != 0)
		{
			auto owner = World::GetActorByUID(components.back()->GetOwnerUID());
			owner->RemoveComponent(components.back().get());
		}

		components.pop_back();

		//Make sure the Properties Dock is reset else you'll have widgets trying to access invalid pointers to components.
		Editor::Get().ClearProperties();
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
			if (components[i]->IsActive() && components[i]->IsTickEnabled())
			{
				components[i]->Tick(deltaTime);
			}
		}
	}

	T* GetFirstComponent()
	{
		return components.front().get();
	}

	T* GetLastComponent()
	{
		return components.back().get();
	}

	T* GetComponentByUID(UID uid)
	{
		for (auto& component : components)
		{
			if (component->GetUID() == uid)
			{
				return component.get();
			}
		}

		return nullptr;
	}

	T* GetComponentByName(std::string name)
	{
		for (auto& component : components)
		{
			if (component->name == name)
			{
				return component.get();
			}
		}

		return nullptr;
	}

	virtual void Serialise(Serialiser& s) override
	{
		s.WriteLine(VString::stows(_name));
		s.WriteLine(components.size());

		for (auto& component : components)
		{
			s.WriteLine(component->GetOwnerUID());
			s.WriteLine(VString::stows(component->GetName()));

			auto props = component->GetProps();
			s.Serialise(props);

			s.WriteLine(L"next");
		}
	}

	virtual void SerialiseBinary(BinarySerialiser& s) override
	{
		s.WriteString(_name);
		size_t numComponents = components.size();
		s.Write(&numComponents);

		for (auto& component : components)
		{
			UID ownerUID = component->GetOwnerUID();
			s.Write(&ownerUID);
			s.WriteString(component->GetName());

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

	virtual void DestroyAll() override
	{
		for (auto& component : components)
		{
			component->Destroy();
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
			outComponents.emplace_back(component.get());
		}
		return outComponents;
	}

	size_t GetNumComponents() override
	{
		return components.size();
	}

	bool Empty() override
	{
		return components.empty();
	}

	virtual Component* FindComponentByName(std::string componentName) override
	{
		for (auto& component : components)
		{
			if (component->GetName() == componentName)
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
virtual void Remove() override { system.Remove(GetIndex()); } \

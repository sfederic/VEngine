#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include "Core/Transform.h"
#include "Core/Properties.h"
#include "Core/UID.h"

class Component;
struct SpatialComponent;
class IActorSystem;

using namespace DirectX;

class Actor
{
public:
	Actor() {}

	XMMATRIX GetWorldMatrix();
	void UpdateTransform(const XMMATRIX parentWorld);
	XMMATRIX GetTransformMatrix();
	void SetTransform(const Transform transform);
	Transform GetTransform();

	XMFLOAT3 GetPosition();
	XMVECTOR GetPositionV();
	XMVECTOR GetHomogeneousPositionV();

	XMFLOAT3 GetScale();
	XMVECTOR GetScaleV();

	XMFLOAT4 GetRotation();
	XMVECTOR GetRotationV();

	void SetPosition(const XMVECTOR position);
	void SetPosition(const XMFLOAT3 position);
	void SetScale(const XMVECTOR scale);
	void SetRotation(const XMVECTOR rotation);

	void AddPositionV(const XMVECTOR offset);
	void AddRotation(XMVECTOR direction, float angle);

	//All vector functions return in world space, not local.
	XMFLOAT3 GetForwardVector();
	XMVECTOR GetForwardVectorV();
	XMFLOAT3 GetRightVector();
	XMVECTOR GetRightVectorV();
	XMFLOAT3 GetUpVector();
	XMVECTOR GetUpVectorV();

	virtual Properties GetProps() = 0;
	std::vector<Properties> GetAllProps();

	//called before all Start()'s
	virtual void Awake() {}

	//called on gameplay start
	virtual void Start() {}

	//Called on gameplay end (level load, editor stop and start)
	virtual void End() {}

	//Called after Start()
	virtual void LateStart() {}

	//Called once per frame to update actor and its components
	virtual void Tick(float deltaTime) {}

	//An optional function to call after components have been setup (Not Create()'ed, after the Actor's constructor has been called.
	//(Use this when you want to override component fields that have to be defined in constructors)
	virtual void Create() {}

	//Create function called after all components are created.
	virtual void PostCreate() {}

	//Do not override this direcly. ACTOR_SYSTEM macro overrides instead.
	virtual void Destroy() {}

	//Iterates over every actor from the actor's system to avoid a rename collision. bool denotes if collision occured.
	bool SetName(const std::string newName);

	//Use this for when world state update order is an issue (e.g. ActorSystem, serialisation, WorldEditor)
	void SimpleSetName(const std::string newName) { name = newName; }

	std::string GetName() { return name; }

	std::string GetTypeName();

	//This shouldn't be called too often, only when ComponentSystem::Init() can't be called.
	void CreateAllComponents();

	//Set Actor and all components as active/inactive. Active state sets actor components as equivalent.
	void SetActive(bool newActive);

	bool IsActive() { return active; }

	void SetVisibility(bool visibility) { visible = visibility; }
	bool IsVisible() { return visible; }

	void SetTickEnabled(bool enabled);
	inline bool IsTickEnabled() { return tickEnabled; }

	//Set Actor and components active field as opposite of what it currently is.
	void ToggleActive();

	void AddChild(Actor* actor);

	void AddComponent(Component* component);

	void RemoveComponent(std::string componentName);

	void RemoveComponent(Component* componentToRemove);

	void SetEmptyRootComponent();

	SpatialComponent& GetRootComponent() { return *rootComponent; }

	Component* FindComponentAllowNull(const std::string componentName);

	template <typename T>
	T* CreateComponent(std::string componentName, T component = T())
	{
		return T::system.Add(componentName, this, std::move(component));
	}

	std::vector<Component*> GetAllComponents();

	template <typename T>
	std::vector<T*> GetComponentsOfType()
	{
		std::vector<T*> outComponents;

		for (auto& componentPair : componentMap)
		{
			T* outComponent = dynamic_cast<T*>(componentPair.second);
			if (outComponent)
			{
				outComponents.emplace_back(outComponent);
			}
		}

		return outComponents;
	}

	template <typename T>
	T* GetFirstComponentOfTypeAllowNull()
	{
		std::vector<T*> componentsOfType = GetComponentsOfType<T>();
		if (!componentsOfType.empty())
		{
			return componentsOfType.front();
		}

		return nullptr;
	}

	template <typename T>
	T* GetComponent(std::string componentName)
	{
		auto componentIt = componentMap.find(componentName);
		if (componentIt != componentMap.end())
		{
			return static_cast<T*>(componentIt->second);
		}

		return nullptr;
	}

	Component* GetComponentByUID(UID componentUID);
	Component* GetComponentByName(const std::string componentName);

	void ResetOwnerUIDToComponents();

	UID GetUID() { return uid; }
	void SetUID(const UID uid_) { uid = uid_; }

	int GetSystemIndex() { return actorSystemIndex; }
	void SetSystemIndex(const int index) { actorSystemIndex = index; }

	IActorSystem* GetActorSystem() { return actorSystem; }
	void SetActorSystem(IActorSystem* system) { actorSystem = system; }

	//Helper for whether the Actor's mesh can be occluded by player and camera.
	//This function is needed because player to camera transparency can mess with previously hit actors
	//for example if they're Destroy()ed in battle or by whatever else.
	bool CanBeTransparentlyOccluded();

	bool HasTag(const std::string& tag);
	void AddTag(const std::string& tag) { tags.insert(tag); }

	void SetMeshesToDynamicPhysicsState();
	void SetMeshesToStaticPhysicsState();

protected:
	Actor* parent = nullptr;
	std::vector<Actor*> children;
	std::set<std::string> tags;
	std::map<std::string, Component*> componentMap;
	SpatialComponent* rootComponent = nullptr;
	IActorSystem* actorSystem = nullptr;
	std::string name;
	UID uid = GenerateUID();
	int actorSystemIndex = -1;
	bool active = true;
	bool visible = true;
	bool tickEnabled = true;
};

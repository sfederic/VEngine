#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include "Transform.h"
#include "Properties.h"
#include "UID.h"
#include "Log.h"

struct Component;
struct SpatialComponent;
class IActorSystem;

using namespace DirectX;

class Actor
{
public:
	Actor* parent = nullptr;
	std::vector<Actor*> children;

	std::set<std::string> tags;

	std::map<std::string, Component*> componentMap;

	SpatialComponent* rootComponent = nullptr;

private:
	IActorSystem* actorSystem = nullptr;

	std::string name;

	UID uid = GenerateUID();

	bool active = true;

	int actorSystemIndex = -1;

public:
	//GAME SPECIFIC VARS

public:
	Actor() {}

	XMMATRIX GetWorldMatrix();
	void UpdateTransform(XMMATRIX parentWorld);
	XMMATRIX GetTransformMatrix();
	void SetTransform(Transform transform);
	Transform GetTransform();

	XMFLOAT3 GetPosition();
	XMVECTOR GetPositionV();
	XMVECTOR GetHomogeneousPositionV();

	XMFLOAT3 GetScale();
	XMVECTOR GetScaleV();

	XMFLOAT4 GetRotation();
	XMVECTOR GetRotationV();

	void SetPosition(XMVECTOR position);
	void SetPosition(XMFLOAT3 position);
	void SetScale(XMVECTOR scale);
	void SetRotation(XMVECTOR rotation);

	XMFLOAT3 GetForwardVector();
	XMVECTOR GetForwardVectorV();
	XMFLOAT3 GetRightVector();
	XMVECTOR GetRightVectorV();
	XMFLOAT3 GetUpVector();
	XMVECTOR GetUpVectorV();

	virtual Properties GetProps() = 0;
	std::vector<Properties> GetAllProps();

	//called before all Start()'s
	virtual void Awake();

	//called on gameplay start
	virtual void Start();

	//Called on gameplay end (level load, editor stop and start)
	virtual void End() {}

	//Called after Start()
	virtual void LateStart();

	//Called once per frame to update actor and its components
	virtual void Tick(float deltaTime);

	//An optional function to call after components have been setup, and after Actor's constructor
	virtual void Create();

	//Do not override this direcly. ACTOR_SYSTEM macro overrides instead.
	virtual void Destroy();

	//Iterates over every actor from the actor's system to avoid a rename collision. bool denotes if collision occured.
	bool SetName(std::string newName);

	//Use this for when world state update order is an issue (e.g. ActorSystem, serialisation, WorldEditor)
	void SimpleSetName(std::string newName) { name = newName; }

	std::string GetName() { return name; }

	//This shouldn't be called too often, only when ComponentSystem::Init() can't be called.
	void CreateAllComponents();

	//Set Actor and all components as active/inactive. Active state sets actor components as equivalent.
	void SetActive(bool newActive);

	inline bool IsActive() { return active; }

	//Set Actor and components active field as opposite of what it currently is.
	void ToggleActive();

	void AddChild(Actor* actor);

	void AddComponent(Component* component);

	void RemoveComponent(std::string componentName);

	void RemoveComponent(Component* componentToRemove);

	void SetEmptyRootComponent();

	Component* FindComponentAllowNull(std::string componentName);

	template <typename T>
	T* CreateComponent(T component, std::string componentName)
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
				outComponents.push_back(outComponent);
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
	T* GetComponentByNameAndType(std::string componentName)
	{
		for (auto& componentPair : componentMap)
		{
			if (componentPair.second->name == componentName)
			{
				return static_cast<T*>(componentPair.second);
			}
		}

		Log("Component [%s] not found on Actor [%s].", componentName.c_str(), this->name.c_str());
		return nullptr;
	}

	Component* GetComponentByName(std::string componentName);

	void ResetOwnerUIDToComponents();

	UID GetUID() { return uid; }
	void SetUID(UID uid_) { uid = uid_; }

	int GetSystemIndex() { return actorSystemIndex; }
	void SetSystemIndex(int index) { actorSystemIndex = index; }

	IActorSystem* GetActorSystem() { return actorSystem; }
	void SetActorSystem(IActorSystem* system) { actorSystem = system; }
};

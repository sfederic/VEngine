#pragma once
#include <vector>
#include <string>
#include <set>
#include "Transform.h"
#include "Properties.h"
#include "UID.h"
#include "Log.h"

struct Component;
struct SpatialComponent;
class IActorSystem;

using namespace DirectX;

struct Actor
{
	Actor* parent = nullptr;
	std::vector<Actor*> children;

	std::set<std::string> tags;

	SpatialComponent* rootComponent = nullptr;
	std::vector<Component*> components;

	IActorSystem* actorSystem = nullptr;

	std::string name;

	//The index of the Actor to its ActorSystem
	int index = -1;

	UID uid = GenerateUID();

private:
	bool active = true;

public:
	//Constructor for actors is called when world is loaded in editor.
	Actor();

	XMMATRIX GetWorldMatrix();
	void UpdateTransform(XMMATRIX parentWorld);
	XMMATRIX GetTransformMatrix();
	void SetTransform(Transform transform);
	Transform GetTransform();

	XMFLOAT3 GetPosition();
	XMVECTOR GetPositionVector();
	XMVECTOR GetHomogeneousPositionVector();

	XMFLOAT3 GetScale();
	XMVECTOR GetScaleVector();

	XMFLOAT4 GetRotation();
	XMVECTOR GetRotationVector();

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

	//This shouldn't be called too often, only when ComponentSystem::Init() can't be called.
	void CreateAllComponents();

	//Set Actor and all components as active/inactive. Active state sets actor components as equivalent.
	void SetActive(bool newActive);

	inline bool IsActive() { return active; }

	//Set Actor and components active field as opposite of what it currently is.
	void ToggleActive();

	void AddChild(Actor* actor);

	template <typename T>
	std::vector<T*> GetComponentsOfType()
	{
		std::vector<T*> outComponents;

		for (Component* component : components)
		{
			T* outComponent = dynamic_cast<T*>(component);
			if (outComponent)
			{
				outComponents.push_back(outComponent);
			}
		}

		return outComponents;
	}

	template <typename T>
	T* GetComponentByNameAndType(std::string componentName)
	{
		for (auto component : components)
		{
			if (component->name == componentName)
			{
				return static_cast<T*>(component);
			}
		}

		Log("Component [%s] not found on Actor [%s].", componentName.c_str(), this->name.c_str());
		return nullptr;
	}

	Component* GetComponentByName(std::string componentName);

	void ResetOwnerUIDToComponents();
};

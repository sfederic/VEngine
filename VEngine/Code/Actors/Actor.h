#pragma once
#include <vector>
#include <string>
#include <set>
#include <DirectXCollision.h>
#include "Transform.h"
#include "Properties.h"
#include "UID.h"

struct Component;
struct SpatialComponent;
struct IActorSystem;

using namespace DirectX;

struct Actor
{
	Actor* parent = nullptr;
	std::vector<Actor*> children;

	std::set<std::string, std::string> tags;

	SpatialComponent* rootComponent = nullptr;
	std::vector<Component*> components;

	IActorSystem* actorSystem = nullptr;

	std::string name;

	int index = -1;
	UID uid = 0;

	bool active = true;

	Actor();
	XMMATRIX GetWorldMatrix();
	void UpdateTransform(XMMATRIX parentWorld);
	XMMATRIX GetTransformMatrix();
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
	void SetTransform(Transform transform);
	Transform GetTransform();
	XMFLOAT3 GetForwardVector();
	XMVECTOR GetForwardVectorV();
	XMFLOAT3 GetRightVector();
	XMVECTOR GetRightVectorV();
	XMFLOAT3 GetUpVector();
	XMVECTOR GetUpVectorV();
	virtual Properties GetProps() = 0;
	std::vector<Properties> GetAllProps();
	virtual void Start();
	virtual void Tick(float deltaTime);
	virtual void Create();
	virtual void Destroy();

	//Iterates over every actor from the actor's system to avoid a rename collision. bool denotes if collision occured.
	bool SetName(std::string newName);

	//This shouldn't be called too often, only when ComponentSystem::Init() can't be called.
	void CreateAllComponents();

	//Set Actor and all components as active/inactive. Active state sets actor components as equivalent.
	void SetActive(bool newActive);

	//Set Actor and components active field as opposite of what it currently is.
	void ToggleActive();

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
};

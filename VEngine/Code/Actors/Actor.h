#pragma once
#include <vector>
#include <string>
#include <DirectXCollision.h>
#include "Transform.h"
#include "Properties.h"

struct Component;
struct SpatialComponent;
struct IActorSystem;

using namespace DirectX;

struct Actor
{
	Actor* parent = nullptr;
	std::vector<Actor*> children;

	SpatialComponent* rootComponent = nullptr;
	std::vector<Component*> components;

	IActorSystem* actorSystem = nullptr;

	std::string name;

	int index = -1;

	Actor();
	XMMATRIX GetWorldMatrix();
	void UpdateTransform(XMMATRIX parentWorld);
	XMMATRIX GetTransformMatrix();
	XMFLOAT3 GetPosition();
	XMVECTOR GetPositionVector();
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
	XMFLOAT3 GetRightVector();
	XMFLOAT3 GetUpVector();
	virtual Properties GetProps() = 0;
	virtual void Tick(double deltaTime);
	virtual void Destroy();

	//Iterates over every actor from the actor's system to avoid a rename collision. bool denotes if collision occured.
	bool SetName(std::string newName);

	//This shouldn't be called too often, only when ComponentSystem::Init() can't be called
	void CreateAllComponents();

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

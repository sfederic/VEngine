#pragma once
#include <vector>
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

	IActorSystem* actorSystem;

	Actor();
	XMMATRIX GetWorldMatrix();
	void UpdateTransform(XMMATRIX parentWorld);
	XMMATRIX GetTransformMatrix();
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetScale();
	XMFLOAT4 GetRotation();
	void SetPosition(XMVECTOR position);
	void SetScale(XMVECTOR scale);
	void SetRotation(XMVECTOR rotation);
	void SetTransform(Transform transform);
	Transform GetTransform();
	virtual Properties GetProps();
};

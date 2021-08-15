#pragma once
#include <vector>
#include <DirectXCollision.h>
#include "Transform.h"

struct Component;
struct SpatialComponent;

using namespace DirectX;

struct Actor
{
	Actor* parent = nullptr;
	std::vector<Actor*> children;

	SpatialComponent* rootComponent = nullptr;
	std::vector<Component*> components;

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
};

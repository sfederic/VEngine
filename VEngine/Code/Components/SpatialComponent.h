#pragma once
#include <vector>
#include <DirectXCollision.h>
#include "Component.h"
#include "Transform.h"

using namespace DirectX;

struct SpatialComponent : Component
{
	Transform transform;
	BoundingOrientedBox boundingBox;
	SpatialComponent* parent = nullptr;
	std::vector<SpatialComponent*> children;

	void AddChild(SpatialComponent* component);
	XMMATRIX GetWorldMatrix();
	void UpdateTransform(XMMATRIX parentWorld = XMMatrixIdentity());
	void SetPosition(XMFLOAT3 newPosition);
	void SetPosition(XMVECTOR newPosition);
	XMFLOAT3 GetForwardVector();
	XMFLOAT3 GetRightVector();
	XMFLOAT3 GetUpVector();
};

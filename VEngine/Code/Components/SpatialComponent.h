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
	XMFLOAT3 GetPosition();
	void SetPosition(XMFLOAT3 newPosition);
	void SetPosition(XMVECTOR newPosition);
	XMFLOAT3 GetScale();
	void SetScale(XMFLOAT3 newScale);
	void SetScale(XMVECTOR newScale);
	XMFLOAT4 GetRotation();
	void SetRotation(XMFLOAT4 newRotation);
	void SetRotation(XMVECTOR newRotation);
	XMFLOAT3 GetForwardVector();
	XMVECTOR GetForwardVectorV();
	XMFLOAT3 GetRightVector();
	XMVECTOR GetRightVectorV();
	XMFLOAT3 GetUpVector();
	XMVECTOR GetUpVectorV();
};

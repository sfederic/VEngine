#pragma once
#include <vector>
#include <DirectXCollision.h>
#include "Component.h"
#include "Core/Transform.h"
#include "Physics/CollisionLayers.h"

using namespace DirectX;

struct SpatialComponent : Component
{
	Transform transform;
	BoundingOrientedBox boundingBox;
	SpatialComponent* parent = nullptr;
	std::vector<SpatialComponent*> children;

	CollisionLayers layer = CollisionLayers::All;

	Properties GetProps() override;

	void AddChild(SpatialComponent* component);
	void RemoveChild(SpatialComponent* component);
	XMMATRIX GetWorldMatrix();
	void UpdateTransform(XMMATRIX parentWorld = XMMatrixIdentity());

	XMFLOAT3 GetLocalPosition();
	XMVECTOR GetLocalPositionV();
	XMVECTOR GetWorldPositionV();
	void SetLocalPosition(float x, float y, float z);
	void SetLocalPosition(XMFLOAT3 newPosition);
	void SetLocalPosition(XMVECTOR newPosition);
	void SetWorldPosition(XMVECTOR position);
	void AddLocalPosition(XMVECTOR offset);
	void AddWorldPosition(XMVECTOR offset);

	XMFLOAT3 GetLocalScale();
	XMVECTOR GetLocalScaleV();
	void SetLocalScale(float uniformScale);
	void SetLocalScale(float x, float y, float z);
	void SetLocalScale(XMFLOAT3 newScale);
	void SetLocalScale(XMVECTOR newScale);

	void SetWorldScale(float uniformScale);
	void SetWorldScale(XMVECTOR scale);

	XMFLOAT4 GetLocalRotation();
	XMVECTOR GetLocalRotationV();
	void SetLocalRotation(float x, float y, float z, float w);
	void SetLocalRotation(XMFLOAT4 newRotation);
	void SetLocalRotation(XMVECTOR newRotation);
	void SetWorldRotation(XMVECTOR newRotation);
	void AddLocalRotation(XMVECTOR vector, float angle);

	XMFLOAT3 GetForwardVector();
	XMVECTOR GetForwardVectorV();
	XMFLOAT3 GetRightVector();
	XMVECTOR GetRightVectorV();
	XMFLOAT3 GetUpVector();
	XMVECTOR GetUpVectorV();
};

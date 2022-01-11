#pragma once
#include <vector>
#include <DirectXCollision.h>
#include "Component.h"
#include "Transform.h"
#include "Physics/CollisionLayers.h"

using namespace DirectX;

//@Todo: SpatialComponent isn't really working with both local and world transforms, neither is Actor.
//Need to split the Get/Set functions up to include both.

struct SpatialComponent : Component
{
	Transform transform;
	BoundingOrientedBox boundingBox;
	SpatialComponent* parent = nullptr;
	std::vector<SpatialComponent*> children;

	CollisionLayers layer = CollisionLayers::All;

	void AddChild(SpatialComponent* component);
	void RemoveChild(SpatialComponent* component);
	XMMATRIX GetWorldMatrix();
	void UpdateTransform(XMMATRIX parentWorld = XMMatrixIdentity());

	XMFLOAT3 GetPosition();
	XMVECTOR GetPositionV();
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 newPosition);
	void SetPosition(XMVECTOR newPosition);

	XMFLOAT3 GetScale();
	XMVECTOR GetScaleV();
	void SetScale(float x, float y, float z);
	void SetScale(XMFLOAT3 newScale);
	void SetScale(XMVECTOR newScale);

	XMFLOAT4 GetRotation();
	XMVECTOR GetRotationV();
	void SetRotation(float x, float y, float z, float w);
	void SetRotation(XMFLOAT4 newRotation);
	void SetRotation(XMVECTOR newRotation);
	void SetWorldRotation(XMVECTOR newRotation);

	XMFLOAT3 GetForwardVector();
	XMVECTOR GetForwardVectorV();
	XMFLOAT3 GetRightVector();
	XMVECTOR GetRightVectorV();
	XMFLOAT3 GetUpVector();
	XMVECTOR GetUpVectorV();

	XMMATRIX GetTransformMatrix();
};

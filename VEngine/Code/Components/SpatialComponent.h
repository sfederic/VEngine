#pragma once

#include <vector>
#include <DirectXCollision.h>
#include "Component.h"
#include "Core/Transform.h"
#include "Physics/CollisionLayers.h"

using namespace DirectX;

class SpatialComponent : public Component
{
public:
	//@Todo: there's a lot of direct refs to this via properties and whatever else. Look into making it protected completely.
	Transform transform;

	Properties GetProps() override;

	XMMATRIX GetWorldMatrix();
	void UpdateTransform(XMMATRIX parentWorld = XMMatrixIdentity());

	XMFLOAT3 GetLocalPosition();
	XMVECTOR GetLocalPositionV();
	XMFLOAT3 GetWorldPosition();
	XMVECTOR GetWorldPositionV();
	void SetLocalPosition(float x, float y, float z);
	void SetLocalPosition(XMFLOAT3 newPosition);
	void SetLocalPosition(XMVECTOR newPosition);
	void SetWorldPosition(XMFLOAT3 position);
	void SetWorldPosition(XMVECTOR position);
	void AddLocalPosition(XMVECTOR offset);
	void AddWorldPosition(XMVECTOR offset);

	XMFLOAT3 GetLocalScale();
	XMVECTOR GetLocalScaleV();
	XMFLOAT3 GetWorldScale();
	XMVECTOR GetWorldScaleV();
	void SetLocalScale(float uniformScale);
	void SetLocalScale(float x, float y, float z);
	void SetLocalScale(XMFLOAT3 newScale);
	void SetLocalScale(XMVECTOR newScale);

	void SetWorldScale(float uniformScale);
	void SetWorldScale(XMVECTOR scale);

	XMFLOAT4 GetLocalRotation();
	XMVECTOR GetLocalRotationV();
	XMFLOAT4 GetWorldRotation();
	XMVECTOR GetWorldRotationV();
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

	auto GetBoundingBox() { return boundingBox; }
	XMVECTOR GetBoundsExtents() { return XMLoadFloat3(&boundingBox.Extents); }
	XMVECTOR GetBoundsCenter() { return XMLoadFloat3(&boundingBox.Center); }
	void SetBoundsExtents(XMFLOAT3 extents) { boundingBox.Extents = extents; }

	auto GetTransform() { return transform; }
	void SetTransform(const Transform& transform_) { transform = transform_; }

	auto GetParent() { return parent; }
	void SetParent(SpatialComponent* newParent) { parent = newParent; }

	auto GetChildren() { return children; }
	void AddChild(SpatialComponent* component);
	void RemoveChild(SpatialComponent* component);

	auto GetCollisionLayer() { return layer; }
	void SetCollisionLayer(CollisionLayers layer_) { layer = layer_; }

protected:
	void Pitch(float angle);
	void RotateY(float angle);
	void FPSCameraRotation();

	BoundingOrientedBox boundingBox;
	SpatialComponent* parent = nullptr;
	std::vector<SpatialComponent*> children;

	CollisionLayers layer = CollisionLayers::All;
};

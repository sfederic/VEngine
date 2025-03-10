#include <DirectXCollision.h>

export module Components.SpatialComponent;

import Core.Transform;
import Physics.CollisionLayers;

export class SpatialComponent : public Component
{
public:
	Transform transform;

	Properties GetProps() override;

	DirectX::XMMATRIX GetWorldMatrix() const;

	DirectX::XMFLOAT3 GetLocalPosition() const;
	DirectX::XMVECTOR GetLocalPositionV() const;
	DirectX::XMFLOAT3 GetWorldPosition() const;
	DirectX::XMVECTOR GetWorldPositionV() const;
	void SetLocalPosition(float x, float y, float z);
	void SetLocalPosition(DirectX::XMFLOAT3 newPosition);
	void SetLocalPosition(DirectX::XMVECTOR newPosition);
	void SetWorldPosition(DirectX::XMFLOAT3 position);
	void SetWorldPosition(DirectX::XMVECTOR position);
	void AddLocalPosition(DirectX::XMVECTOR offset);
	void AddWorldPosition(DirectX::XMVECTOR offset);

	DirectX::XMFLOAT3 GetLocalScale() const;
	DirectX::XMVECTOR GetLocalScaleV() const;
	DirectX::XMFLOAT3 GetWorldScale() const;
	DirectX::XMVECTOR GetWorldScaleV() const;
	void SetLocalScale(float uniformScale);
	void SetLocalScale(float x, float y, float z);
	void SetLocalScale(DirectX::XMFLOAT3 newScale);
	void SetLocalScale(DirectX::XMVECTOR newScale);

	void SetWorldScale(float uniformScale);
	void SetWorldScale(DirectX::XMFLOAT3 scale);
	void SetWorldScale(DirectX::XMVECTOR scale);

	DirectX::XMFLOAT4 GetLocalRotation() const;
	DirectX::XMVECTOR GetLocalRotationV() const;
	DirectX::XMFLOAT4 GetWorldRotation() const;
	DirectX::XMVECTOR GetWorldRotationV() const;
	void SetLocalRotation(float x, float y, float z, float w);
	void SetLocalRotation(DirectX::XMFLOAT4 newRotation);
	void SetLocalRotation(DirectX::XMVECTOR newRotation);
	void SetWorldRotation(DirectX::XMVECTOR newRotation);
	void AddLocalRotation(DirectX::XMVECTOR vector, float angle);
	void AddWorldRotation(DirectX::XMVECTOR vector, float angle);

	DirectX::XMFLOAT3 GetForwardVector() const;
	DirectX::XMVECTOR GetForwardVectorV() const;
	DirectX::XMFLOAT3 GetRightVector() const;
	DirectX::XMVECTOR GetRightVectorV() const;
	DirectX::XMFLOAT3 GetUpVector() const;
	DirectX::XMVECTOR GetUpVectorV() const;

	DirectX::BoundingOrientedBox GetBoundsInWorldSpace();
	DirectX::BoundingOrientedBox GetBounds() const { return boundingBox; }
	DirectX::XMVECTOR GetBoundsExtents() const { return DirectX::XMLoadFloat3(&boundingBox.Extents); }
	DirectX::XMVECTOR GetBoundsCenter() const { return DirectX::XMLoadFloat3(&boundingBox.Center); }
	void SetBoundsExtents(DirectX::XMFLOAT3 extents) { boundingBox.Extents = extents; }

	auto GetTransform() const { return transform; }
	void SetTransform(const Transform& transform_) { transform = transform_; }

	auto GetParent() { return parent; }
	void SetParent(SpatialComponent* newParent) { parent = newParent; }

	auto GetChildren() { return children; }
	void AddChild(SpatialComponent* component);
	void RemoveChild(SpatialComponent* component);

	auto GetCollisionLayer() const { return layer; }
	void SetCollisionLayer(CollisionLayers layer_) { layer = layer_; }

protected:
	void Pitch(float angle);
	void RotateY(float angle);
	void FPSCameraRotation();

	DirectX::BoundingOrientedBox boundingBox;
	std::vector<SpatialComponent*> children;
	SpatialComponent* parent = nullptr;
	CollisionLayers layer = CollisionLayers::All;

private:
	DirectX::XMMATRIX GetParentWorldMatrix(DirectX::XMMATRIX parentWorld) const;
};

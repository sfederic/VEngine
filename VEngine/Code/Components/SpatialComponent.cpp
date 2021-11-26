#include "SpatialComponent.h"

void SpatialComponent::AddChild(SpatialComponent* component)
{
	assert(component);
	component->parent = this;
	children.push_back(component);
}

XMMATRIX SpatialComponent::GetWorldMatrix()
{
	XMMATRIX parentWorld = XMMatrixIdentity();

	if (parent)
	{
		parentWorld = parent->GetWorldMatrix();
	}

	UpdateTransform(parentWorld);

	return transform.world;
}

void SpatialComponent::UpdateTransform(XMMATRIX parentWorld)
{
	XMMATRIX world = transform.GetAffine() * parentWorld;

	for (SpatialComponent* child : children)
	{
		child->UpdateTransform(world);
	}

	transform.world = world;
}

XMFLOAT3 SpatialComponent::GetPosition()
{
	return transform.position;
}

XMVECTOR SpatialComponent::GetPositionV()
{
	XMVECTOR position = XMLoadFloat3(&transform.position);
	return position;
}

void SpatialComponent::SetPosition(float x, float y, float z)
{
	transform.position = XMFLOAT3(x, y, z);
}

void SpatialComponent::SetPosition(XMFLOAT3 newPosition)
{
	transform.position = newPosition;
}

void SpatialComponent::SetPosition(XMVECTOR newPosition)
{
	XMStoreFloat3(&transform.position, newPosition);
}

XMFLOAT3 SpatialComponent::GetScale()
{
	return transform.scale;
}

XMVECTOR SpatialComponent::GetScaleV()
{
	XMVECTOR scale = XMLoadFloat3(&transform.scale);
	return scale;
}

void SpatialComponent::SetScale(float x, float y, float z)
{
	transform.scale = XMFLOAT3(x, y, z);
}

void SpatialComponent::SetScale(XMFLOAT3 newScale)
{
	transform.scale = newScale;
}

void SpatialComponent::SetScale(XMVECTOR newScale)
{
	XMStoreFloat3(&transform.scale, newScale);
}

XMVECTOR SpatialComponent::GetRotationV()
{
	XMVECTOR rotation = XMLoadFloat4(&transform.rotation);
	return rotation;
}

void SpatialComponent::SetRotation(float x, float y, float z, float w)
{
	transform.rotation = XMFLOAT4(x, y, z, w);
}

XMFLOAT4 SpatialComponent::GetRotation()
{
	return transform.rotation;
}

void SpatialComponent::SetRotation(XMFLOAT4 newRotation)
{
	transform.rotation = newRotation;
}

void SpatialComponent::SetRotation(XMVECTOR newRotation)
{
	XMStoreFloat4(&transform.rotation, newRotation);
}

XMFLOAT3 SpatialComponent::GetForwardVector()
{
	XMFLOAT3 forward;
	XMStoreFloat3(&forward, transform.world.r[2]);
	return forward;
}

XMVECTOR SpatialComponent::GetForwardVectorV()
{
	return XMVector3Normalize(transform.world.r[2]);
}

XMFLOAT3 SpatialComponent::GetRightVector()
{
	XMFLOAT3 right;
	XMStoreFloat3(&right, transform.world.r[0]);
	return right;
}

XMVECTOR SpatialComponent::GetRightVectorV()
{
	return XMVector3Normalize(transform.world.r[0]);
}

XMFLOAT3 SpatialComponent::GetUpVector()
{
	XMFLOAT3 up;
	XMStoreFloat3(&up, transform.world.r[1]);
	return up;
}

XMVECTOR SpatialComponent::GetUpVectorV()
{
	return XMVector3Normalize(transform.world.r[1]);
}

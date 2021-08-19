#include "Actor.h"
#include "Components/SpatialComponent.h"

Actor::Actor()
{
}

XMMATRIX Actor::GetWorldMatrix()
{
	XMMATRIX parentWorld = XMMatrixIdentity();

	if (parent)
	{
		parentWorld = parent->GetWorldMatrix();
	}

	UpdateTransform(parentWorld);

	return rootComponent->transform.world;
}

void Actor::UpdateTransform(XMMATRIX parentWorld)
{
	XMMATRIX world = GetTransformMatrix() * parentWorld;

	for (Actor* child : children)
	{
		child->UpdateTransform(world);
	}

	rootComponent->transform.world = world;
}

XMMATRIX Actor::GetTransformMatrix()
{
	XMVECTOR rotationOffset = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	if (parent)
	{
		rotationOffset = parent->rootComponent->transform.world.r[3];
	}

	return rootComponent->transform.GetAffineRotationOrigin(rotationOffset);
}

XMFLOAT3 Actor::GetPosition()
{
	return XMFLOAT3(rootComponent->transform.position);
}

XMFLOAT3 Actor::GetScale()
{
	return XMFLOAT3(rootComponent->transform.scale);
}

XMFLOAT4 Actor::GetRotation()
{
	return XMFLOAT4(rootComponent->transform.rotation);
}

void Actor::SetPosition(XMVECTOR position)
{
	XMStoreFloat3(&rootComponent->transform.position, position);
}

void Actor::SetScale(XMVECTOR scale)
{
	XMStoreFloat3(&rootComponent->transform.scale, scale);
}

void Actor::SetRotation(XMVECTOR rotation)
{
	XMStoreFloat4(&rootComponent->transform.rotation, rotation);
}

void Actor::SetTransform(Transform transform)
{
	rootComponent->transform = transform;
}

Transform Actor::GetTransform()
{
	return rootComponent->transform;
}

Properties Actor::GetProps()
{
	Properties props;
	props.Add("Position", &rootComponent->transform.position);
	props.Add("Scale", &rootComponent->transform.scale);
	return props;
}

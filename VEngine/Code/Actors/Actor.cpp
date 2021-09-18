#include "Actor.h"
#include "Actors/IActorSystem.h"
#include "Components/SpatialComponent.h"
#include "World.h"

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

XMVECTOR Actor::GetPositionVector()
{
	XMFLOAT3 pos = GetPosition();
	return XMLoadFloat3(&pos);
}

XMFLOAT3 Actor::GetScale()
{
	return XMFLOAT3(rootComponent->transform.scale);
}

XMVECTOR Actor::GetScaleVector()
{
	XMFLOAT3 scale = GetScale();
	return XMLoadFloat3(&scale);
}

XMFLOAT4 Actor::GetRotation()
{
	return XMFLOAT4(rootComponent->transform.rotation);
}

XMVECTOR Actor::GetRotationVector()
{
	XMFLOAT4 rotation = GetRotation();
	return XMLoadFloat4(&rotation);
}

void Actor::SetPosition(XMVECTOR position)
{
	XMStoreFloat3(&rootComponent->transform.position, position);
}

void Actor::SetPosition(XMFLOAT3 position)
{
	rootComponent->transform.position = position;
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

XMFLOAT3 Actor::GetForwardVector()
{
	XMFLOAT3 forward;
	XMStoreFloat3(&forward, rootComponent->transform.world.r[2]);
	return forward;
}

XMFLOAT3 Actor::GetRightVector()
{
	XMFLOAT3 right;
	XMStoreFloat3(&right, rootComponent->transform.world.r[0]);
	return right;
}

XMFLOAT3 Actor::GetUpVector()
{
	XMFLOAT3 up;
	XMStoreFloat3(&up, rootComponent->transform.world.r[1]);
	return up;
}

Properties Actor::GetProps()
{
	Properties props("Actor");

	props.Add("Position", &rootComponent->transform.position);
	props.Add("Scale", &rootComponent->transform.scale);
	props.Add("Rotation", &rootComponent->transform.rotation);

	return props;
}

std::vector<Properties> Actor::GetAllProps()
{
	std::vector<Properties> propsVector;

	propsVector.push_back(GetProps());

	for (Component* component : components)
	{
		Properties componentProps = component->GetProps();
		propsVector.push_back(componentProps);
	}

	return propsVector;
}

void Actor::Start()
{
}

void Actor::Tick(double deltaTime)
{
}

void Actor::Create()
{
}

void Actor::Destroy()
{
}

bool Actor::SetName(std::string newName)
{
	for (Actor* actor : world.GetAllActorsInWorld())
	{
		if (actor->name == newName)
		{
			return false;
		}
	}

	name = newName;
	return true;
}

void Actor::CreateAllComponents()
{
	for (Component* component : components)
	{
		component->Create();
	}
}

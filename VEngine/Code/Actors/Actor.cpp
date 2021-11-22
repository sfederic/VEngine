#include "Actor.h"
#include "Actors/IActorSystem.h"
#include "Components/SpatialComponent.h"
#include "World.h"
#include "Camera.h"

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

//This is mainly used for Widgets rendering to screen-space.
//Because the W component is needed as-is, there's no Float3 function for this yet.
XMVECTOR Actor::GetHomogeneousPositionVector()
{
	XMMATRIX view = activeCamera->GetViewMatrix();
	XMMATRIX proj = activeCamera->GetProjectionMatrix();
	XMMATRIX mvp = rootComponent->GetWorldMatrix() * view * proj;

	return mvp.r[3];
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
	rootComponent->UpdateTransform();
}

void Actor::SetPosition(XMFLOAT3 position)
{
	rootComponent->transform.position = position;
	rootComponent->UpdateTransform();
}

void Actor::SetScale(XMVECTOR scale)
{
	XMStoreFloat3(&rootComponent->transform.scale, scale);
	rootComponent->UpdateTransform();
}

void Actor::SetRotation(XMVECTOR rotation)
{
	XMStoreFloat4(&rootComponent->transform.rotation, rotation);
	rootComponent->UpdateTransform();
}

void Actor::SetTransform(Transform transform)
{
	rootComponent->transform = transform;
	rootComponent->UpdateTransform();
}

Transform Actor::GetTransform()
{
	return rootComponent->transform;
}

XMFLOAT3 Actor::GetForwardVector()
{
	XMFLOAT3 forward;
	XMStoreFloat3(&forward, XMVector3Normalize(rootComponent->transform.world.r[2]));
	return forward;
}

XMVECTOR Actor::GetForwardVectorV()
{
	return XMVector3Normalize(rootComponent->transform.world.r[2]);
}

XMFLOAT3 Actor::GetRightVector()
{
	XMFLOAT3 right;
	XMStoreFloat3(&right, XMVector3Normalize(rootComponent->transform.world.r[0]));
	return right;
}

XMVECTOR Actor::GetRightVectorV()
{
	return XMVector3Normalize(rootComponent->transform.world.r[0]);
}

XMFLOAT3 Actor::GetUpVector()
{
	XMFLOAT3 up;
	XMStoreFloat3(&up, XMVector3Normalize(rootComponent->transform.world.r[1]));
	return up;
}

XMVECTOR Actor::GetUpVectorV()
{
	return XMVector3Normalize(rootComponent->transform.world.r[1]);
}

Properties Actor::GetProps()
{
	Properties props("Actor");

	props.Add("Position", &rootComponent->transform.position);
	props.Add("Scale", &rootComponent->transform.scale);
	props.Add("Rotation", &rootComponent->transform.rotation);
	props.Add("UID", &uid).readOnly = true;

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

void Actor::Tick(float deltaTime)
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

void Actor::SetActive(bool newActiveValue)
{
	active = newActiveValue;

	for (auto component : components)
	{
		component->active = newActiveValue;
	}
}

void Actor::ToggleActive()
{
	active = !active;

	for (auto component : components)
	{
		component->active = !active;
	}
}

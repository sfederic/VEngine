#include "vpch.h"
#include "Actor.h"
#include "Components/MeshComponent.h"
#include "Components/EmptyComponent.h"
#include "Core/World.h"
#include "Core/Log.h"
#include "Core/Camera.h"
#include "Physics/PhysicsSystem.h"

XMMATRIX Actor::GetWorldMatrix() const
{
	return rootComponent->GetWorldMatrix();
}

XMFLOAT3 Actor::GetPosition() const
{
	return rootComponent->GetWorldPosition();
}

XMVECTOR Actor::GetPositionV() const
{
	XMFLOAT3 pos = GetPosition();
	return XMLoadFloat3(&pos);
}

//This is mainly used for Widgets rendering to screen-space.
//Because the W component is needed as-is, there's no Float3 function for this yet.
XMVECTOR Actor::GetHomogeneousPositionV() const
{
	auto& activeCamera = Camera::GetActiveCamera();
	XMMATRIX view = activeCamera.GetViewMatrix();
	XMMATRIX proj = activeCamera.GetProjectionMatrix();
	XMMATRIX mvp = rootComponent->GetWorldMatrix() * view * proj;

	return mvp.r[3];
}

XMFLOAT3 Actor::GetScale() const
{
	return rootComponent->GetWorldScale();
}

XMVECTOR Actor::GetScaleV() const
{
	XMFLOAT3 scale = GetScale();
	return XMLoadFloat3(&scale);
}

XMFLOAT4 Actor::GetRotation() const
{
	return rootComponent->GetWorldRotation();
}

XMVECTOR Actor::GetRotationV() const
{
	XMFLOAT4 rotation = GetRotation();
	return XMLoadFloat4(&rotation);
}

void Actor::SetPosition(const XMVECTOR position)
{
	rootComponent->SetWorldPosition(position);
}

void Actor::SetPosition(const XMFLOAT3 position)
{
	rootComponent->SetWorldPosition(position);
}

void Actor::AddPositionV(const XMVECTOR offset)
{
	SetPosition(GetPositionV() + offset);
}

void Actor::SetScale(const XMVECTOR scale)
{
	rootComponent->SetWorldScale(scale);
}

void Actor::SetRotation(const XMVECTOR rotation)
{
	rootComponent->SetWorldRotation(rotation);
}

void Actor::AddRotation(XMVECTOR direction, float angle)
{
	rootComponent->AddLocalRotation(direction, angle);
}

void Actor::SetTransform(const Transform transform)
{
	rootComponent->SetTransform(transform);
}

Transform Actor::GetTransform() const
{
	return rootComponent->GetTransform();
}

XMFLOAT3 Actor::GetForwardVector() const
{
	XMFLOAT3 forward;
	XMStoreFloat3(&forward, XMVector3Normalize(rootComponent->GetWorldMatrix().r[2]));
	return forward;
}

XMVECTOR Actor::GetForwardVectorV() const
{
	return XMVector3Normalize(rootComponent->GetWorldMatrix().r[2]);
}

XMFLOAT3 Actor::GetRightVector() const
{
	XMFLOAT3 right;
	XMStoreFloat3(&right, XMVector3Normalize(rootComponent->GetWorldMatrix().r[0]));
	return right;
}

XMVECTOR Actor::GetRightVectorV() const
{
	return XMVector3Normalize(rootComponent->GetWorldMatrix().r[0]);
}

XMFLOAT3 Actor::GetUpVector() const
{
	XMFLOAT3 up;
	XMStoreFloat3(&up, XMVector3Normalize(rootComponent->GetWorldMatrix().r[1]));
	return up;
}

XMVECTOR Actor::GetUpVectorV() const
{
	return XMVector3Normalize(rootComponent->GetWorldMatrix().r[1]);
}

Properties Actor::GetProps()
{
	Properties props("Actor");

	props.ownerUID = GetUID();
	//Got the whitespace here because std::map orders by key, don't have time to make an ordered_map,
	//meaning the whitespace will keep the transform values at the top of the props dock.
	props.Add(" Position", &rootComponent->transform.position);
	props.Add(" Scale", &rootComponent->transform.scale);
	props.Add(" Rotation", &rootComponent->transform.rotation);
	props.Add("UID", &uid).hide = true;
	props.Add("Name", &_name).hide = true;
	props.Add(" Enabled", &active);
	props.Add(" Visible", &visible);

	return props;
}

std::vector<Component*> Actor::GetAllComponents()
{
	std::vector<Component*> result;
	for (auto& componentPair : componentMap)
	{
		result.emplace_back(componentPair.second);
	}
	return result;
}

std::vector<Properties> Actor::GetAllProps()
{
	std::vector<Properties> propsVector;
	propsVector.emplace_back(GetProps());

	for (auto& componentPair : componentMap)
	{
		Properties componentProps = componentPair.second->GetProps();
		propsVector.emplace_back(componentProps);
	}

	return propsVector;
}

void Actor::DeferDestroy()
{
	deferredForDestroy = true;
	actorSystem->DeferActorForDestroy(actorSystemIndex);
}

bool Actor::SetName(const std::string newName)
{
	//Check for duplicate names in world
	if (!World::CheckIfActorExistsInWorld(newName))
	{
		World::RemoveActorFromWorld(this);
		_name = newName;
		World::AddActorToWorld(this);

		return true;
	}

	return false;
}

std::string Actor::GetTypeName()
{
	return actorSystem->GetName();
}

void Actor::CreateAllComponents()
{
	for (auto& componentPair : componentMap)
	{
		componentPair.second->Create();
	}
}

void Actor::SetActive(bool newActiveValue)
{
	active = newActiveValue;

	for (auto& componentPair : componentMap)
	{
		componentPair.second->SetActive(newActiveValue);
	}
}

void Actor::SetVisibility(bool visibility)
{
	visible = visibility;

	for (auto& [name, component] : componentMap)
	{
		component->SetVisibility(visible);
	}
}

void Actor::SetTickEnabled(bool enabled)
{
	tickEnabled = enabled;
}

void Actor::ToggleActive()
{
	active = !active;
	SetActive(active);
}

void Actor::ToggleVisibility()
{
	visible = !visible;
	SetVisibility(visible);
}

void Actor::AddComponent(Component* component)
{
	for (auto& componentPair : componentMap)
	{
		assert(componentPair.second != component);
	}

	component->SetOwnerUID(uid);

	assert(componentMap.find(component->GetName()) == componentMap.end() && "Duplicate Component name (Actor::Create() might be being called twice).");
	componentMap.emplace(component->GetName(), component);
}

void Actor::RemoveComponent(Component* componentToRemove)
{
	componentMap.erase(componentToRemove->GetName());
}

bool Actor::CheckComponentExists(std::string componentName)
{
	return componentMap.find(componentName) != componentMap.end();
}

void Actor::SetRootComponent(SpatialComponent* component)
{
	assert(component);
	rootComponent = component;
}

void Actor::AddChildToRoot(SpatialComponent* child)
{
	rootComponent->AddChild(child);
}

void Actor::SetEmptyRootComponent()
{
	rootComponent = CreateComponent<EmptyComponent>("Root");
}

Component* Actor::FindComponentAllowNull(const std::string componentName)
{
	auto it = componentMap.find(componentName);
	if (it == componentMap.end())
	{
		Log("Component [%s] not found on Actor [%s] in FindComponentAllowNull",
			componentName.c_str(), GetName().c_str());
		return nullptr;
	}
	return it->second;
}

Component* Actor::GetComponentByUID(UID componentUID)
{
	for (auto& [name, component] : componentMap)
	{
		if (component->GetUID() == componentUID)
		{
			return component;
		}
	}

	Log("Component [%d] not found on Actor [%s].", componentUID, _name.c_str());
	return nullptr;
}

Component* Actor::GetComponentByName(const std::string componentName)
{
	for (auto& componentPair : componentMap)
	{
		if (componentPair.second->GetName() == componentName)
		{
			return componentPair.second;
		}
	}

	Log("Component [%s] not found on Actor [%s].", componentName.c_str(), _name.c_str());
	return nullptr;
}

void Actor::ResetOwnerUIDToComponents()
{
	for (auto& componentPair : componentMap)
	{
		componentPair.second->SetOwnerUID(uid);
	}
}

bool Actor::CanBeTransparentlyOccluded()
{
	auto mesh = GetFirstComponentOfTypeAllowNull<MeshComponent>();
	if (mesh && mesh->transparentOcclude)
	{
		return true;
	}

	return false;
}

bool Actor::HasTag(const std::string& tag)
{
	return tags.find(tag) != tags.end();
}

void Actor::SetMeshesToDynamicPhysicsState()
{
	for (auto mesh : GetComponents<MeshComponent>())
	{
		PhysicsSystem::ReleasePhysicsActor(mesh);
		mesh->SetPhysicsStatic(false);
		PhysicsSystem::CreatePhysicsActor(mesh, PhysicsType::Dynamic);
	}
}

void Actor::SetMeshesToStaticPhysicsState()
{
	for (auto mesh : GetComponents<MeshComponent>())
	{
		PhysicsSystem::ReleasePhysicsActor(mesh);
		mesh->SetPhysicsStatic(true);
		PhysicsSystem::CreatePhysicsActor(mesh, PhysicsType::Static);
	}
}

#include "vpch.h"
#include "SpatialComponent.h"

void SpatialComponent::AddChild(SpatialComponent* component)
{
	assert(component != this);
	assert(component);
	component->parent = this;
	children.push_back(component);
}

void SpatialComponent::RemoveChild(SpatialComponent* component)
{
	for (int i = 0; i < children.size(); i++)
	{
		if (children[i] == component)
		{
			children.erase(children.begin() + i);
			return;
		}
	}

	throw new std::exception("child not found");
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

Properties SpatialComponent::GetProps()
{
	//@Todo: think long and hard about adding transform props to spatialcomponent for other components
	//Properties props("SpatialComponent");
	//props.Add("pos", &transform.position);
	//props.Add("rot", &transform.rotation);
	//props.Add("scale", &transform.scale);
	//return props;
	return __super::GetProps();
}

XMFLOAT3 SpatialComponent::GetPosition()
{
	return transform.position;
}

XMVECTOR SpatialComponent::GetWorldPositionV()
{
	return GetWorldMatrix().r[3];
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
	XMStoreFloat3(&forward, XMVector3Normalize(GetWorldMatrix().r[2]));
	return forward;
}

XMVECTOR SpatialComponent::GetForwardVectorV()
{
	return XMVector3Normalize(GetWorldMatrix().r[2]);
}

XMFLOAT3 SpatialComponent::GetRightVector()
{
	XMFLOAT3 right;
	XMStoreFloat3(&right, XMVector3Normalize(GetWorldMatrix().r[0]));
	return right;
}

XMVECTOR SpatialComponent::GetRightVectorV()
{
	return XMVector3Normalize(GetWorldMatrix().r[0]);
}

void SpatialComponent::SetWorldRotation(XMVECTOR newRotation)
{
	XMVECTOR relativeRotation = newRotation;

	if (parent)
	{
		//Looks like relative rotations are inversed with quaternions. ParentQuat(-1) * newRot;
		XMVECTOR parentRot = XMQuaternionInverse(parent->GetRotationV());
		relativeRotation = XMQuaternionMultiply(parentRot, newRotation);
	}

	SetRotation(relativeRotation);
}

XMFLOAT3 SpatialComponent::GetUpVector()
{
	XMFLOAT3 up;
	XMStoreFloat3(&up, XMVector3Normalize(GetWorldMatrix().r[1]));
	return up;
}

XMVECTOR SpatialComponent::GetUpVectorV()
{
	return XMVector3Normalize(GetWorldMatrix().r[1]);
}

#include "vpch.h"
#include "SpatialComponent.h"
#include "Core/VMath.h"
#include "Editor/Editor.h"

void SpatialComponent::AddChild(SpatialComponent* component)
{
	assert(component != this);
	assert(component);
	component->parent = this;
	children.emplace_back(component);
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
	auto props = __super::GetProps();
	props.title = "SpatialComponent";
	props.Add(" Pos", &transform.position);
	props.Add(" Rot", &transform.rotation);
	props.Add(" Scale", &transform.scale);
	return props;
}

XMFLOAT3 SpatialComponent::GetLocalPosition()
{
	return transform.position;
}

XMFLOAT3 SpatialComponent::GetWorldPosition()
{
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, GetWorldPositionV());
	return pos;
}

XMVECTOR SpatialComponent::GetWorldPositionV()
{
	return GetWorldMatrix().r[3];
}

XMVECTOR SpatialComponent::GetLocalPositionV()
{
	XMVECTOR position = XMLoadFloat3(&transform.position);
	return position;
}

void SpatialComponent::SetLocalPosition(float x, float y, float z)
{
	transform.position = XMFLOAT3(x, y, z);
	UpdateTransform();
}

void SpatialComponent::SetLocalPosition(XMFLOAT3 newPosition)
{
	transform.position = newPosition;
	UpdateTransform();
}

void SpatialComponent::SetLocalPosition(XMVECTOR newPosition)
{
	XMStoreFloat3(&transform.position, newPosition);
	UpdateTransform();
}

void SpatialComponent::SetWorldPosition(XMFLOAT3 position)
{
	SetWorldPosition(XMLoadFloat3(&position));
	UpdateTransform();
}

void SpatialComponent::SetWorldPosition(XMVECTOR position)
{
	XMVECTOR relativePosition = position;

	if (parent)
	{
		XMVECTOR parentPosition = parent->GetLocalPositionV();
		relativePosition = XMVectorSubtract(relativePosition, parentPosition);
	}

	SetLocalPosition(relativePosition);
}

void SpatialComponent::AddLocalPosition(XMVECTOR offset)
{
	SetLocalPosition(GetLocalPositionV() + offset);
}

void SpatialComponent::AddWorldPosition(XMVECTOR offset)
{
	SetWorldPosition(GetWorldPositionV() + offset);
}

XMFLOAT3 SpatialComponent::GetLocalScale()
{
	return transform.scale;
}

XMVECTOR SpatialComponent::GetLocalScaleV()
{
	return XMLoadFloat3(&transform.scale);
}

XMFLOAT3 SpatialComponent::GetWorldScale()
{
	XMFLOAT3 scale;
	XMStoreFloat3(&scale, GetWorldScaleV());
	return scale;
}

XMVECTOR SpatialComponent::GetWorldScaleV()
{
	XMVECTOR relativeScale = GetLocalScaleV();

	if (parent)
	{
		XMVECTOR parentScale = parent->GetWorldScaleV();
		relativeScale = XMVectorMultiply(relativeScale, parentScale);
	}

	return relativeScale;
}

void SpatialComponent::SetLocalScale(float uniformScale)
{
	transform.scale = XMFLOAT3(uniformScale, uniformScale, uniformScale);
	UpdateTransform();
}

void SpatialComponent::SetLocalScale(float x, float y, float z)
{
	transform.scale = XMFLOAT3(x, y, z);
	UpdateTransform();
}

void SpatialComponent::SetLocalScale(XMFLOAT3 newScale)
{
	transform.scale = newScale;
	UpdateTransform();
}

void SpatialComponent::SetLocalScale(XMVECTOR newScale)
{
	XMStoreFloat3(&transform.scale, newScale);
	UpdateTransform();
}

void SpatialComponent::SetWorldScale(float uniformScale)
{
	SetWorldScale(XMVectorSet(uniformScale, uniformScale, uniformScale, 0.f));
}

void SpatialComponent::SetWorldScale(XMVECTOR scale)
{
	XMVECTOR relativeScale = scale;

	if (parent)
	{
		XMVECTOR parentScale = parent->GetLocalScaleV();
		relativeScale = XMVectorMultiply(relativeScale, parentScale);
	}

	SetLocalScale(relativeScale);
}

XMVECTOR SpatialComponent::GetLocalRotationV()
{
	return XMLoadFloat4(&transform.rotation);
}

XMFLOAT4 SpatialComponent::GetWorldRotation()
{
	XMFLOAT4 rot;
	XMStoreFloat4(&rot, GetWorldRotationV());
	return rot;
}

XMVECTOR SpatialComponent::GetWorldRotationV()
{
	XMVECTOR relativeRotation = GetLocalRotationV();

	if (parent)
	{
		//Relative rotations are inversed with quaternions, i.e. ParentQuat(-1) * localRotation;
		XMVECTOR parentRot = XMQuaternionInverse(parent->GetWorldRotationV());
		relativeRotation = XMQuaternionMultiply(parentRot, relativeRotation);
	}

	return relativeRotation;
}

void SpatialComponent::SetLocalRotation(float x, float y, float z, float w)
{
	transform.rotation = XMFLOAT4(x, y, z, w);
	UpdateTransform();
}

XMFLOAT4 SpatialComponent::GetLocalRotation()
{
	return transform.rotation;
}

void SpatialComponent::SetLocalRotation(XMFLOAT4 newRotation)
{
	transform.rotation = newRotation;
	UpdateTransform();
}

void SpatialComponent::SetLocalRotation(XMVECTOR newRotation)
{
	XMStoreFloat4(&transform.rotation, newRotation);
	UpdateTransform();
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
		//Relative rotations are inversed with quaternions, i.e. ParentQuat(-1) * localRotation;
		XMVECTOR parentRot = XMQuaternionInverse(parent->GetLocalRotationV());
		relativeRotation = XMQuaternionMultiply(parentRot, newRotation);
	}

	SetLocalRotation(relativeRotation);
}

void SpatialComponent::AddLocalRotation(XMVECTOR vector, float angle)
{
	auto newRotation = 
		XMQuaternionMultiply(GetLocalRotationV(),
			DirectX::XMQuaternionRotationAxis(vector, XMConvertToRadians(angle)));
	SetLocalRotation(newRotation);
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

void SpatialComponent::Pitch(float angle)
{
	//The RightFromQuat is important here as GetRightVector() grabs the global directional vector, 
	//meaning the wall crawling mechanic would mess up FPS controls.
	const XMMATRIX r = XMMatrixRotationAxis(VMath::RightFromQuat(GetLocalRotationV()), angle);
	XMVECTOR q = XMQuaternionMultiply(GetLocalRotationV(), XMQuaternionRotationMatrix(r));

	float roll = 0.f, pitch = 0.f, yaw = 0.f;
	VMath::PitchYawRollFromQuaternion(roll, pitch, yaw, q);
	pitch = XMConvertToDegrees(pitch);
	if (pitch > 80.f || pitch < -80.f)
	{
		return;
	}

	SetLocalRotation(q);
}

void SpatialComponent::RotateY(float angle)
{
	const XMMATRIX r = XMMatrixRotationY(angle);
	const XMVECTOR q = XMQuaternionMultiply(GetLocalRotationV(), XMQuaternionRotationMatrix(r));
	SetLocalRotation(q);
}

void SpatialComponent::FPSCameraRotation()
{
	const int x = editor->centerOffsetX;
	const int y = editor->centerOffsetY;

	const float dx = -XMConvertToRadians(0.25f * (float)x);
	const float dy = -XMConvertToRadians(0.25f * (float)y);

	Pitch(dy);
	RotateY(dx);
}

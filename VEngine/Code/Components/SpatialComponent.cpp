import Components.SpatialComponent;
import Core.VMath;
import Core.Transform;
import Editor.Editor;
import <cassert>;
import <vector>;

import <DirectXMath.h>;

using namespace DirectX;

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

XMMATRIX SpatialComponent::GetWorldMatrix() const
{
	XMMATRIX parentWorld = XMMatrixIdentity();
	if (parent)
	{
		parentWorld = parent->GetWorldMatrix();
	}
	return GetParentWorldMatrix(parentWorld);
}

XMMATRIX SpatialComponent::GetParentWorldMatrix(XMMATRIX parentWorld) const
{
	XMMATRIX world = transform.GetAffine() * parentWorld;
	for (SpatialComponent* child : children)
	{
		child->GetParentWorldMatrix(world);
	}
	return world;
}

Properties SpatialComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add(" Pos", &transform.position);
	props.Add(" Rot", &transform.rotation);
	props.Add(" Scale", &transform.scale);
	return props;
}

XMFLOAT3 SpatialComponent::GetLocalPosition() const
{
	return transform.position;
}

XMFLOAT3 SpatialComponent::GetWorldPosition() const
{
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, GetWorldPositionV());
	return pos;
}

XMVECTOR SpatialComponent::GetWorldPositionV() const
{
	return GetWorldMatrix().r[3];
}

XMVECTOR SpatialComponent::GetLocalPositionV() const
{
	XMVECTOR position = XMLoadFloat3(&transform.position);
	return position;
}

void SpatialComponent::SetLocalPosition(float x, float y, float z)
{
	transform.position = XMFLOAT3(x, y, z);
}

void SpatialComponent::SetLocalPosition(XMFLOAT3 newPosition)
{
	transform.position = newPosition;
}

void SpatialComponent::SetLocalPosition(XMVECTOR newPosition)
{
	XMStoreFloat3(&transform.position, newPosition);
}

void SpatialComponent::SetWorldPosition(XMFLOAT3 position)
{
	SetWorldPosition(XMLoadFloat3(&position));
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

XMFLOAT3 SpatialComponent::GetLocalScale() const
{
	return transform.scale;
}

XMVECTOR SpatialComponent::GetLocalScaleV() const
{
	return XMLoadFloat3(&transform.scale);
}

XMFLOAT3 SpatialComponent::GetWorldScale() const
{
	XMFLOAT3 scale;
	XMStoreFloat3(&scale, GetWorldScaleV());
	return scale;
}

XMVECTOR SpatialComponent::GetWorldScaleV() const
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
}

void SpatialComponent::SetLocalScale(float x, float y, float z)
{
	transform.scale = XMFLOAT3(x, y, z);
}

void SpatialComponent::SetLocalScale(XMFLOAT3 newScale)
{
	transform.scale = newScale;
}

void SpatialComponent::SetLocalScale(XMVECTOR newScale)
{
	XMStoreFloat3(&transform.scale, newScale);
}

void SpatialComponent::SetWorldScale(float uniformScale)
{
	SetWorldScale(XMVectorSet(uniformScale, uniformScale, uniformScale, 1.f));
}

void SpatialComponent::SetWorldScale(XMFLOAT3 scale)
{
	SetWorldScale(XMLoadFloat3(&scale));
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

XMVECTOR SpatialComponent::GetLocalRotationV() const
{
	return XMLoadFloat4(&transform.rotation);
}

XMFLOAT4 SpatialComponent::GetWorldRotation() const
{
	XMFLOAT4 rot;
	XMStoreFloat4(&rot, GetWorldRotationV());
	return rot;
}

XMVECTOR SpatialComponent::GetWorldRotationV() const
{
	XMVECTOR relativeRotation = GetLocalRotationV();

	if (parent)
	{
		//Note: originally was under the impression here that relative rotations are inversed with quaternions
		//i.e. ParentQuat(-1) * localRotation; but it looks like that isn't so. Leaving this comment here just
		//in case rotations decide to blow up one day.
		XMVECTOR parentRot = parent->GetWorldRotationV();
		relativeRotation = XMQuaternionMultiply(parentRot, relativeRotation);
	}

	return relativeRotation;
}

void SpatialComponent::SetLocalRotation(float x, float y, float z, float w)
{
	transform.rotation = XMFLOAT4(x, y, z, w);
}

XMFLOAT4 SpatialComponent::GetLocalRotation() const
{
	return transform.rotation;
}

void SpatialComponent::SetLocalRotation(XMFLOAT4 newRotation)
{
	transform.rotation = newRotation;
}

void SpatialComponent::SetLocalRotation(XMVECTOR newRotation)
{
	XMStoreFloat4(&transform.rotation, newRotation);
}

XMFLOAT3 SpatialComponent::GetForwardVector() const
{
	XMFLOAT3 forward;
	XMStoreFloat3(&forward, XMVector3Normalize(GetWorldMatrix().r[2]));
	return forward;
}

XMVECTOR SpatialComponent::GetForwardVectorV() const
{
	return XMVector3Normalize(GetWorldMatrix().r[2]);
}

XMFLOAT3 SpatialComponent::GetRightVector() const
{
	XMFLOAT3 right;
	XMStoreFloat3(&right, XMVector3Normalize(GetWorldMatrix().r[0]));
	return right;
}

XMVECTOR SpatialComponent::GetRightVectorV() const
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

void SpatialComponent::AddWorldRotation(XMVECTOR vector, float angle)
{
	auto newRotation =
		XMQuaternionMultiply(GetWorldRotationV(),
			DirectX::XMQuaternionRotationAxis(vector, XMConvertToRadians(angle)));
	SetWorldRotation(newRotation);
}

XMFLOAT3 SpatialComponent::GetUpVector() const
{
	XMFLOAT3 up;
	XMStoreFloat3(&up, XMVector3Normalize(GetWorldMatrix().r[1]));
	return up;
}

XMVECTOR SpatialComponent::GetUpVectorV() const
{
	return XMVector3Normalize(GetWorldMatrix().r[1]);
}

BoundingOrientedBox SpatialComponent::GetBoundsInWorldSpace()
{
	const XMMATRIX worldMatrix = GetWorldMatrix();
	BoundingOrientedBox outBounds;
	boundingBox.Transform(outBounds, worldMatrix);
	return outBounds;
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
	const int x = Editor::Get().GetCenterOffsetX();
	const int y = Editor::Get().GetCenterOffsetX();

	const float dx = -XMConvertToRadians(0.25f * (float)x);
	const float dy = -XMConvertToRadians(0.25f * (float)y);

	Pitch(dy);
	RotateY(dx);
}

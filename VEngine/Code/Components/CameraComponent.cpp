#include <DirectXMath.h>

import Components.CameraComponent;
import Core.VMath;
import Core.Core;
import Render.Renderer;
import Core.Camera;

using namespace DirectX;

CameraComponent::CameraComponent(XMFLOAT3 startPos)
{
	SetLocalPosition(startPos);
}

XMMATRIX CameraComponent::GetViewMatrix()
{
	const auto worldMatrix = GetWorldMatrix();
	const auto pos = GetWorldPositionV();
	auto focus = pos + GetForwardVectorV();

	if (targetActor != nullptr)
	{
		focus = targetActor->GetPositionV();
	}

	auto view = DirectX::XMMatrixLookAtLH(pos, focus, VMath::GlobalUpVector());

	//Camera translation shaking
	const auto shakeVector = Shake();
	view.r[3] += shakeVector;

	return view;
}

XMMATRIX CameraComponent::GetProjectionMatrix()
{
	const float FOVRadian = XMConvertToRadians(FOV);
	return XMMatrixPerspectiveFovLH(FOVRadian, Renderer::Get().GetAspectRatio(), nearZ, farZ);
}

void CameraComponent::Move(float d, XMVECTOR axis)
{
	const XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR position = GetLocalPositionV();
	position = XMVectorMultiplyAdd(s, axis, position);
	SetLocalPosition(position);
}

void CameraComponent::ZoomTo(Actor* actor)
{
	XMVECTOR forward = GetForwardVectorV();

	//Trace the camera down the line its pointing towards the actor
	XMVECTOR actorPos = actor->GetPositionV();
	XMVECTOR zoomPos = actorPos - (forward * 5.f);

	SetLocalPosition(zoomPos);
}

void CameraComponent::SetLerpToFocusPoint(bool value)
{
	lerpToFocusPoint = value;
	lerpToFocusPointPercent = 0.f;
}

//Only works with translation for now.
//Ref: https://gdcvault.com/play/1023146/Math-for-Game-Programmers-Juicing
XMVECTOR CameraComponent::Shake()
{
	if (shakeLevel <= 0.f)
	{
		return XMVectorZero();
	}

	shakeLevel -= Core::GetDeltaTime();

	const float range = VMath::RandomRange(-1.f, 1.f);

	const float maxShake = 0.1f;
	const float xOffset = maxShake * shakeLevel * range;
	const float yOffset = maxShake * shakeLevel * range;
	const float zOffset = maxShake * shakeLevel * range;

	return XMVectorSet(xOffset, yOffset, zOffset, 0.f); //Make sure the w here is 0
}

void CameraComponent::SetAsActiveCamera()
{
	Camera::SetActiveCamera(this);
}

Properties CameraComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("FOV", &FOV);
	props.Add("Near Z", &nearZ);
	props.Add("Far Z", &farZ);
	return props;
}

#include "vpch.h"
#include "CameraComponent.h"
#include <algorithm>
#include "Core/VMath.h"
#include "Core/Core.h"
#include "Render/Renderer.h"
#include "Editor/Editor.h"
#include "Core/Camera.h"

CameraComponent::CameraComponent(XMFLOAT3 startPos)
{
	SetLocalPosition(startPos);
	UpdateTransform();
}

XMMATRIX CameraComponent::GetViewMatrix()
{
	XMVECTOR worldPos = GetWorldPositionV();

	if (targetActor)
	{
		focusPoint = targetActor->GetPositionV();
	}
	else
	{
		focusPoint = worldPos + GetForwardVectorV();
	}

	XMMATRIX view = XMMatrixLookAtLH(worldPos, focusPoint, GetUpVectorV());

	//Camera translation shaking
	XMVECTOR shakeVector = Shake();
	view.r[3] += shakeVector;

	return view;
}

XMMATRIX CameraComponent::GetProjectionMatrix()
{
	const float FOVRadian = XMConvertToRadians(FOV);
	return XMMatrixPerspectiveFovLH(FOVRadian, Renderer::GetAspectRatio(), nearZ, farZ);
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

//Only works with translation for now.
//Ref: https://gdcvault.com/play/1023146/Math-for-Game-Programmers-Juicing
//@Todo: there's a lot more to do here to give the camera shake a better falloff. Look at ref for ideas.
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
	activeCamera = this;
}

Properties CameraComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = "CameraComponent";
	props.Add("FOV", &FOV);
	props.Add("Near Z", &nearZ);
	props.Add("Far Z", &farZ);
	return props;
}

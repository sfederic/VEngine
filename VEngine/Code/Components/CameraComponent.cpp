#include "vpch.h"
#include <random>
#include "CameraComponent.h"
#include "VMath.h"
#include "Core.h"
#include "Actors/Actor.h"
#include "Render/Renderer.h"

CameraComponent::CameraComponent(XMFLOAT3 startPos)
{
	transform.position = startPos;
	UpdateTransform();
}

XMMATRIX CameraComponent::GetViewMatrix()
{
	XMVECTOR forward = GetForwardVectorV();
	XMVECTOR position = GetPositionV();

	XMMATRIX view = XMMatrixIdentity();

	if (targetActor)
	{
		focusPoint = targetActor->GetPositionVector();
	}

	view = XMMatrixLookAtLH(transform.world.r[3], focusPoint, VMath::XMVectorUp());

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

void CameraComponent::Pitch(float angle)
{
	const XMMATRIX r = XMMatrixRotationAxis(GetRightVectorV(), angle);
	const XMVECTOR q = XMQuaternionMultiply(GetRotationV(), XMQuaternionRotationMatrix(r));
	SetRotation(q);
}

void CameraComponent::RotateY(float angle)
{
	const XMMATRIX r = XMMatrixRotationY(angle);
	const XMVECTOR q = XMQuaternionMultiply(GetRotationV(), XMQuaternionRotationMatrix(r));
	SetRotation(q);
}

void CameraComponent::Move(float d, XMVECTOR axis)
{
	const XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR position = GetPositionV();
	position = XMVectorMultiplyAdd(s, axis, position);
	SetPosition(position);
}

void CameraComponent::ZoomTo(Actor* actor)
{
	XMVECTOR forward = GetForwardVectorV();

	//Trace the camera down the line its pointing towards the actor
	XMVECTOR actorPos = actor->GetPositionVector();
	XMVECTOR zoomPos = actorPos - (forward * 5.f);

	SetPosition(zoomPos);
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

	std::random_device randomDevice;
	std::mt19937 randomGenerator(randomDevice());
	std::uniform_real_distribution<float> uidDist(-1.f, 1.f);

	const float maxShake = 0.1f;
	const float xOffset = maxShake * shakeLevel * uidDist(randomGenerator);
	const float yOffset = maxShake * shakeLevel * uidDist(randomGenerator);
	const float zOffset = maxShake * shakeLevel * uidDist(randomGenerator);

	return XMVectorSet(xOffset, yOffset, zOffset, 0.f); //Make sure the w here is 0
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

void CameraComponent::FrustumCull()
{
	for (auto actor : World::GetAllActorsInWorld())
	{
		XMMATRIX cameraView = GetViewMatrix();
		XMVECTOR cameraViewDet = XMMatrixDeterminant(cameraView);
		XMMATRIX invView = XMMatrixInverse(&cameraViewDet, cameraView);

		XMMATRIX actorWorld = actor->GetWorldMatrix();
		XMVECTOR actorWorldDet = XMMatrixDeterminant(actorWorld);
		XMMATRIX invWorld = XMMatrixInverse(&actorWorldDet, actorWorld);

		XMMATRIX viewToLocal = XMMatrixMultiply(invView, invWorld);

		XMMATRIX proj = GetProjectionMatrix();
		BoundingFrustum frustum, localSpaceFrustum;
		BoundingFrustum::CreateFromMatrix(frustum, proj);
		frustum.Transform(localSpaceFrustum, viewToLocal);

		if (localSpaceFrustum.Contains(actor->rootComponent->boundingBox) == DirectX::DISJOINT)
		{
			actor->SetActive(false);
		}
		else
		{
			actor->SetActive(true);
		}
	}
}

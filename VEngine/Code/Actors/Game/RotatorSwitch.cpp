#include "vpch.h"
#include "RotatorSwitch.h"
#include "Core/World.h"
#include "Core/VMath.h"
#include "Components/MeshComponent.h"

RotatorSwitch::RotatorSwitch()
{
	isDestructible = false;
}

void RotatorSwitch::Start()
{
	__super::Start();

	actorToRotate = World::GetActorByName(actorToRotateName);
	nextRotation = actorToRotate->GetRotationV();
}

void RotatorSwitch::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (!rotationFinished)
	{
		XMVECTOR currentRotation = actorToRotate->GetRotationV();
		if (!XMQuaternionEqual(currentRotation, nextRotation))
		{
			constexpr float rotationSpeed = 3.33f;
			actorToRotate->SetRotation(
				VMath::QuatConstantLerp(currentRotation, nextRotation, deltaTime, rotationSpeed));
		}
		else
		{
			rotationFinished = true;
		}
	}
}

Properties RotatorSwitch::GetProps()
{
	auto props = __super::GetProps();
	props.title = "RotatorSwitch";
	props.Add("Actor to Rotate", &actorToRotateName);
	props.Add("Rotation Axis", &rotationAxis);
	props.Add("Rotation Increment", &rotationIncrement);
	return props;
}

void RotatorSwitch::Interact()
{
	rotationFinished = false;

	const float angle = XMConvertToRadians(rotationIncrement);
	const XMVECTOR rotationAxisV = XMLoadFloat3(&rotationAxis);
	nextRotation = XMQuaternionMultiply(
		actorToRotate->GetRotationV(), DirectX::XMQuaternionRotationAxis(rotationAxisV, angle));
}

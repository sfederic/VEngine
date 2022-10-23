#include "vpch.h"
#include "RotatorSwitch.h"
#include "World.h"
#include "VMath.h"
#include "Components/MeshComponent.h"

void RotatorSwitch::Start()
{
	actorToRotate = World::GetActorByName(actorToRotateName);
	nextRotation = actorToRotate->GetRotationV();
}

void RotatorSwitch::Tick(float deltaTime)
{
	if (!rotationFinished)
	{
		XMVECTOR currentRotation = actorToRotate->GetRotationV();
		if (!XMQuaternionEqual(currentRotation, nextRotation))
		{
			constexpr float rotationSpeed = 3.33f;
			actorToRotate->SetRotation(
				VMath::QuatConstantLerp(currentRotation, nextRotation, deltaTime, rotationSpeed));
		}
	}
}

Properties RotatorSwitch::GetProps()
{
	auto props = __super::GetProps();
	props.Add("Actor to Rotate", &actorToRotateName);
	return props;
}

void RotatorSwitch::Interact()
{
	rotationFinished = false;

	constexpr float rotationIncrement = 90.f;
	constexpr float angle = XMConvertToRadians(rotationIncrement);

	nextRotation = XMQuaternionMultiply(
		actorToRotate->GetRotationV(), DirectX::XMQuaternionRotationAxis(VMath::GlobalUpVector(), angle));
}

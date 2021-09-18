#include "Player.h"
#include "Components/MeshComponent.h"
#include "Camera.h"
#include "Input.h"
#include "VMath.h"

Player::Player()
{
	mesh = MeshComponent::system.Add(this, MeshComponent("cube.fbx", "wall.png"));
	rootComponent = mesh;

	camera = CameraComponent::system.Add(this, CameraComponent(XMFLOAT3(0.f, 3.f, -7.f), false));
	rootComponent->AddChild(camera);
}

void Player::Start()
{
	currentPos = GetPositionVector();
	nextPos = currentPos;

	currentRot = GetRotationVector();
	nextRot = currentRot;
}

void Player::Tick(double deltaTime)
{
	const float moveSpeed = 7.5f;
	currentPos = VMath::VectorConstantLerp(currentPos, nextPos, deltaTime * moveSpeed);

	if (VMath::VecEqual(currentPos, nextPos))
	{
		if (Input::GetAsyncKey(Keys::W))
		{
			nextPos = GetForwardVectorV() + currentPos;
		}		
		if (Input::GetAsyncKey(Keys::S))
		{
			nextPos = -GetForwardVectorV() + currentPos;
		}
		if (Input::GetAsyncKey(Keys::A))
		{
			nextPos = -GetRightVectorV() + currentPos;
		}
		if (Input::GetAsyncKey(Keys::D))
		{
			nextPos = GetRightVectorV() + currentPos;
		}
	}

	SetPosition(currentPos);

	const float rotSpeed = 45.f;
	currentRot = XMQuaternionSlerp(currentRot, nextRot, deltaTime * rotSpeed);

	if (VMath::VecEqual(currentRot, nextRot))
	{
		if (Input::GetKeyDown(Keys::Right))
		{
			float angle = XMConvertToRadians(90.f);
			nextRot = XMQuaternionMultiply(nextRot, DirectX::XMQuaternionRotationAxis(VMath::XMVectorUp(), angle));
		}		
		if (Input::GetKeyDown(Keys::Left))
		{
			float angle = XMConvertToRadians(-90.f);
			nextRot = XMQuaternionMultiply(nextRot, DirectX::XMQuaternionRotationAxis(VMath::XMVectorUp(), angle));
		}
	}

	SetRotation(currentRot);
}

Properties Player::GetProps()
{
    return Actor::GetProps();
}

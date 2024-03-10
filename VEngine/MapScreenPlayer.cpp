#include "vpch.h"
#include "MapScreenPlayer.h"
#include "Core/Input.h"
#include "Components/CameraComponent.h"
#include "Core/Camera.h"
#include "Core/VMath.h"

MapScreenPlayer::MapScreenPlayer()
{
	SetEmptyRootComponent();

	camera = CreateComponent<CameraComponent>("Camera");
	rootComponent->AddChild(camera);
}

void MapScreenPlayer::Create()
{
	camera->SetLocalPosition(3.f, 3.f, -3.f);
	auto rot = VMath::LookAtRotation(rootComponent->GetWorldPositionV(), camera->GetWorldPositionV());
	camera->SetWorldRotation(rot);
}

void MapScreenPlayer::Start()
{
	Camera::SetActiveCamera(camera);
}

void MapScreenPlayer::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	HandleInput(deltaTime);
}

Properties MapScreenPlayer::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void MapScreenPlayer::HandleInput(float deltaTime)
{
	const float moveSpeed = 10.f * deltaTime;
	const float rotateSpeed = 75.f * deltaTime;

	const XMVECTOR forwardMovementVector = DirectX::XMVector3Cross(
		camera->GetRightVectorV(), VMath::GlobalUpVector());

	if (Input::GetKeyHeld(Keys::W))
	{
		AddPositionV(forwardMovementVector * moveSpeed);
	}
	else if (Input::GetKeyHeld(Keys::S))
	{
		AddPositionV(-forwardMovementVector * moveSpeed);
	}

	if (Input::GetKeyHeld(Keys::A))
	{
		AddPositionV(-camera->GetRightVectorV() * moveSpeed);
	}
	else if (Input::GetKeyHeld(Keys::D))
	{
		AddPositionV(camera->GetRightVectorV() * moveSpeed);
	}

	if (Input::GetKeyHeld(Keys::Right))
	{
		AddRotation(VMath::GlobalUpVector(), rotateSpeed);
	}
	else if (Input::GetKeyHeld(Keys::Left))
	{
		AddRotation(VMath::GlobalUpVector(), -rotateSpeed);
	}

	if (Input::GetKeyHeld(Keys::Up))
	{
		AddRotation(camera->GetRightVectorV(), rotateSpeed);
	}
	else if (Input::GetKeyHeld(Keys::Down))
	{
		AddRotation(camera->GetRightVectorV(), -rotateSpeed);
	}
}

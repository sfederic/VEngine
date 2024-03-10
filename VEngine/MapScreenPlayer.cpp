#include "vpch.h"
#include "MapScreenPlayer.h"
#include "Core/Input.h"
#include "Components/CameraComponent.h"
#include "Core/Camera.h"
#include "Core/VMath.h"

MapScreenPlayer::MapScreenPlayer()
{
	//Give the camera a pivot to rotate around.
	SetEmptyRootComponent();

	camera = CreateComponent<CameraComponent>("Camera");
	rootComponent->AddChild(camera);
}

void MapScreenPlayer::Create()
{
	camera->SetLocalPosition(0.f, 0.f, -3.f);
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
	const float moveSpeed = 15.f * deltaTime;

	if (Input::GetKeyHeld(Keys::W))
	{
		AddPositionV(GetForwardVectorV() * moveSpeed);
	}
	else if (Input::GetKeyHeld(Keys::S))
	{
		AddPositionV(-GetForwardVectorV() * moveSpeed);
	}

	if (Input::GetKeyHeld(Keys::A))
	{
		AddPositionV(-GetRightVectorV() * moveSpeed);
	}
	else if (Input::GetKeyHeld(Keys::D))
	{
		AddPositionV(GetRightVectorV() * moveSpeed);
	}

	const float rotateSpeed = 25.f * deltaTime;

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
		AddRotation(GetRightVectorV(), rotateSpeed);
	}
	else if (Input::GetKeyHeld(Keys::Down))
	{
		AddRotation(GetRightVectorV(), -rotateSpeed);
	}
}

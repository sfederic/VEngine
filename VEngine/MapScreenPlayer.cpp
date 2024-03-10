#include "vpch.h"
#include "MapScreenPlayer.h"
#include "Core/Input.h"
#include "Components/CameraComponent.h"

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

void MapScreenPlayer::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	HandleInput(deltaTime);
}

void MapScreenPlayer::HandleInput(float deltaTime)
{
	if (Input::GetKeyHeld(Keys::W))
	{
		AddPositionV(GetForwardVectorV() * deltaTime);
	}
	else if (Input::GetKeyHeld(Keys::S))
	{
		AddPositionV(-GetForwardVectorV() * deltaTime);
	}

	if (Input::GetKeyHeld(Keys::A))
	{
		AddPositionV(-GetRightVectorV() * deltaTime);
	}
	else if (Input::GetKeyHeld(Keys::D))
	{
		AddPositionV(GetRightVectorV() * deltaTime);
	}
}

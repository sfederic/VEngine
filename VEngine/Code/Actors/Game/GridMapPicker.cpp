#include "vpch.h"
#include "GridMapPicker.h"
#include "Input.h"
#include "Components/CameraComponent.h"
#include "Gameplay/GameUtils.h"

GridMapPicker::GridMapPicker()
{
	SetEmptyRootComponent();
	
	//Camera setup
	camera = CreateComponent(CameraComponent(), "Camera");
	camera->targetActor = this;
	camera->SetPosition(1.75f, 1.75f, -2.75f);
	rootComponent->AddChild(camera);
}

void GridMapPicker::Start()
{
	GameUtils::DisablePlayer();
}

void GridMapPicker::Tick(float deltaTime)
{
}

Properties GridMapPicker::GetProps()
{
	return __super::GetProps();
}

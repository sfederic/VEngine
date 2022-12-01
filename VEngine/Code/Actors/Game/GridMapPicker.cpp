#include "vpch.h"
#include "GridMapPicker.h"
#include "Input.h"
#include "VMath.h"
#include "Actors/Game/GridActor.h"
#include "Components/CameraComponent.h"
#include "UI/Game/GridMapPickerSelectionInfoWidget.h"
#include "Physics/Raycast.h"

GridMapPicker::GridMapPicker()
{
	SetEmptyRootComponent();
	
	//Camera setup
	camera = CreateComponent(CameraComponent(), "Camera");
	camera->targetActor = this;
	camera->SetPosition(1.75f, 1.75f, -2.75f);
	rootComponent->AddChild(camera);

	//Selection Widget setup
	gridMapPickerSelectionInfoWidget = CreateWidget<GridMapPickerSelectionInfoWidget>();
	gridMapPickerSelectionInfoWidget->AddToViewport();
}

void GridMapPicker::Tick(float deltaTime)
{
	DisplayHitActorSelectionInfo();

	if (Input::GetKeyDown(Keys::W))
	{
		AddPositionV(GetForwardVectorV());
	}
	else if (Input::GetKeyDown(Keys::S))
	{
		AddPositionV(-GetForwardVectorV());
	}

	if (Input::GetKeyDown(Keys::A))
	{
		AddPositionV(-GetRightVectorV());
	}
	else if (Input::GetKeyDown(Keys::D))
	{
		AddPositionV(GetRightVectorV());
	}
}

Properties GridMapPicker::GetProps()
{
	return __super::GetProps();
}

void GridMapPicker::DisplayHitActorSelectionInfo()
{
	XMVECTOR pos = GetPositionV();
	XMVECTOR origin = pos + XMVectorSet(0.f, 20.f, 0.f, 0.f); //Set origin high on the y-axis

	Ray ray(this);
	if (Raycast(ray, origin, pos))
	{
		auto gridActor = dynamic_cast<GridActor*>(ray.hitActor);
		if (gridActor)
		{
			SetPosition(gridActor->GetPosition());

			gridMapPickerSelectionInfoWidget->selectedGridActor = gridActor;
		}
	}
	else
	{
		gridMapPickerSelectionInfoWidget->selectedGridActor = nullptr;
	}
}

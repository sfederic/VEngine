#include "vpch.h"
#include "GridMapPicker.h"
#include "Input.h"
#include "Components/CameraComponent.h"
#include "Physics/Raycast.h"

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
}

void GridMapPicker::Tick(float deltaTime)
{
	if (Input::GetMouseLeftUp())
	{
		Ray ray(this);
		if (RaycastFromScreen(ray))
		{
			SetPosition(ray.hitActor->GetPosition());
		}
	}
}

Properties GridMapPicker::GetProps()
{
	return __super::GetProps();
}

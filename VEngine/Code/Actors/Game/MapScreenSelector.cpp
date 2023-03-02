#include "vpch.h"
#include "MapScreenSelector.h"
#include "Components/CameraComponent.h"
#include "Core/Input.h"
#include "Core/VMath.h"
#include "UI/UISystem.h"
#include "UI/Game/MapSelectionInfoWidget.h"
#include "UI/Game/GearSelectionWidget.h"

MapScreenSelector::MapScreenSelector()
{
	SetEmptyRootComponent();
}

void MapScreenSelector::Create()
{
	camera = CreateComponent<CameraComponent>("Camera");
	rootComponent->AddChild(camera);

	camera->SetLocalPosition(0.f, 3.f, -3.f);
	camera->SetWorldRotation(VMath::LookAtRotation(GetPositionV(), camera->GetWorldPositionV()));
}

void MapScreenSelector::Start()
{
	camera->SetAsActiveCamera();

	mapSelectionInfoWidget = UISystem::CreateWidget<MapSelectionInfoWidget>();
	gearSelectionWidget = UISystem::CreateWidget<GearSelectionWidget>();
}

void MapScreenSelector::Tick(float deltaTime)
{
	MovementInput(deltaTime);
	RotationInput(deltaTime);
	ZoomInput(deltaTime);
}

Properties MapScreenSelector::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void MapScreenSelector::SetLeveInfoWidgets(std::wstring_view title, 
	std::wstring_view description, std::string_view levelFilename)
{
	mapSelectionInfoWidget->title = title;
	mapSelectionInfoWidget->description = description;
	mapSelectionInfoWidget->levelToLoad = levelFilename;
	mapSelectionInfoWidget->AddToViewport();

	gearSelectionWidget->AddToViewport();
}

void MapScreenSelector::RemoveLeveInfoWidgets()
{
	mapSelectionInfoWidget->RemoveFromViewport();
	gearSelectionWidget->RemoveFromViewport();
}

void MapScreenSelector::MovementInput(float deltaTime)
{
	const float moveSpeed = 7.5f * deltaTime;

	if (Input::GetKeyHeld(Keys::W))
	{
		AddPositionV(GetForwardVectorV() * moveSpeed);
	}
	else if (Input::GetKeyHeld(Keys::S))
	{
		AddPositionV(GetForwardVectorV() * -moveSpeed);
	}

	if (Input::GetKeyHeld(Keys::A))
	{
		AddPositionV(GetRightVectorV() * -moveSpeed);
	}
	else if (Input::GetKeyHeld(Keys::D))
	{
		AddPositionV(GetRightVectorV() * moveSpeed);
	}
}

void MapScreenSelector::RotationInput(float deltaTime)
{
	const float angle = 100.f * deltaTime;

	if (Input::GetKeyHeld(Keys::Right))
	{
		AddRotation(GetUpVectorV(), angle);
	}
	else if (Input::GetKeyHeld(Keys::Left))
	{
		AddRotation(GetUpVectorV(), -angle);
	}
}

void MapScreenSelector::ZoomInput(float deltaTime)
{
	const float zoomSpeed = 7.5f * deltaTime;

	//Use the distance to the root component as a clamp for zooming camera in and out.
	const float cameraDistToRoot = XMVector3Length(camera->GetWorldPositionV() - GetPositionV()).m128_f32[0];

	if (Input::GetKeyHeld(Keys::Up))
	{
		if (cameraDistToRoot > 1.f)
		{
			camera->AddLocalPosition(camera->GetForwardVectorV() * zoomSpeed);
		}
	}
	else if (Input::GetKeyHeld(Keys::Down))
	{
		if (cameraDistToRoot < 10.f)
		{
			camera->AddLocalPosition(camera->GetForwardVectorV() * -zoomSpeed);
		}
	}
}

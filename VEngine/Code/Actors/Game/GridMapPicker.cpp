#include "vpch.h"
#include "GridMapPicker.h"
#include "Core/Input.h"
#include "Core/VMath.h"
#include "Actors/Game/GridActor.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/Grid.h"
#include "Components/CameraComponent.h"
#include "UI/Game/GridMapPickerSelectionInfoWidget.h"
#include "UI/UISystem.h"
#include "Physics/Raycast.h"
#include "Gameplay/GameUtils.h"

GridMapPicker::GridMapPicker()
{
	SetEmptyRootComponent();
	
	//Camera setup
	camera = CreateComponent<CameraComponent>("Camera");
	camera->targetActor = this;
	camera->SetLocalPosition(1.75f, 1.75f, -2.75f);
	rootComponent->AddChild(camera);

	//Selection Widget setup
	gridMapPickerSelectionInfoWidget = UISystem::CreateWidget<GridMapPickerSelectionInfoWidget>();
	gridMapPickerSelectionInfoWidget->AddToViewport();
}

void GridMapPicker::Start()
{
	nextRotation = GetRotationV();
}

void GridMapPicker::Tick(float deltaTime)
{
	DisplayHitActorSelectionInfo();

	DisplayTrapNodeInformation();

	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRotation, deltaTime, 7.0f));

	RotationInput();

	MovementInput();
}

Properties GridMapPicker::GetProps()
{
	return __super::GetProps();
}

void GridMapPicker::RotationInput()
{
	if (XMQuaternionEqual(GetRotationV(), nextRotation))
	{
		if (Input::GetKeyUp(Keys::Right))
		{
			constexpr float angle = XMConvertToRadians(90.f);
			nextRotation = XMQuaternionMultiply(nextRotation, DirectX::XMQuaternionRotationAxis(VMath::GlobalUpVector(), angle));
		}
		if (Input::GetKeyUp(Keys::Left))
		{
			constexpr float angle = XMConvertToRadians(-90.f);
			nextRotation = XMQuaternionMultiply(nextRotation, DirectX::XMQuaternionRotationAxis(VMath::GlobalUpVector(), angle));
		}
	}
}

void GridMapPicker::MovementInput()
{
	const auto previousPosition = GetPositionV();

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

	//Check if next move in grid is valid, else revert position.
	int nextXGridIndex = std::lroundf(GetPosition().x);
	int nextYGridIndex = std::lroundf(GetPosition().z);

	auto node = Grid::system.GetFirstActor()->GetNodeAllowNull(nextXGridIndex, nextYGridIndex);
	if (node == nullptr)
	{
		SetPosition(previousPosition);
	}
}

void GridMapPicker::DisplayHitActorSelectionInfo()
{
	XMVECTOR pos = GetPositionV();
	XMVECTOR origin = pos + XMVectorSet(0.f, 20.f, 0.f, 0.f); //Set origin high on the y-axis

	HitResult hit(this);
	if (Raycast(hit, origin, pos))
	{
		auto gridActor = dynamic_cast<GridActor*>(hit.hitActor);
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

void GridMapPicker::DisplayTrapNodeInformation()
{
	const int gridIndexX = GetPosition().x;
	const int gridIndexY = GetPosition().z;

	auto grid = Grid::system.GetFirstActor();
	auto node = grid->GetNode(gridIndexX, gridIndexY);

	if (node->trapCard)
	{
		gridMapPickerSelectionInfoWidget->selectedTrapCard = node->trapCard;
	}
	else
	{
		gridMapPickerSelectionInfoWidget->selectedTrapCard = nullptr;
	}
}

void GridMapPicker::ReenablePlayer()
{
	gridMapPickerSelectionInfoWidget->RemoveFromViewport();
	gridMapPickerSelectionInfoWidget->Destroy();

	auto player = Player::system.GetFirstActor();
	GameUtils::SetActiveCamera(player->camera);
	player->SetTickEnabled(true);

	Grid::system.GetFirstActor()->SetActive(false);

	Destroy();
}

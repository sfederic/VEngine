#include "vpch.h"
#include "Player.h"
#include <algorithm>
#include "Core/Input.h"
#include "Core/VMath.h"
#include "Core/Timer.h"
#include "Actors/Game/Enemy.h"
#include "Actors/Game/InteractActor.h"
#include "Actors/MeshSliceActor.h"
#include "Components/MeshComponent.h"
#include "Components/SliceableMeshComponent.h"
#include "Components/CameraComponent.h"
#include "Components/MouseRotateComponent.h"
#include "Components/Lights/PointLightComponent.h"
#include "Components/CharacterControllerComponent.h"
#include "Components/Game/SwordBeam.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/Gears/GearSystem.h"
#include "Gameplay/Gears/Gear.h"
#include "Gameplay/GameplayTags.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/Gears/GearSystem.h"
#include "Gameplay/AttackTypes.h"
#include "Physics/Raycast.h"
#include "UI/Game/PlayerShieldWidget.h"
#include "UI/Game/PlayerReticleWidget.h"
#include "UI/Game/EquippedGearsWidget.h"
#include "UI/UISystem.h"

Player::Player()
{
	characterController = CreateComponent<CharacterControllerComponent>("CharacterController");
	rootComponent = characterController;

	mouseRotateComponent = CreateComponent<MouseRotateComponent>("MouseRotate");
	rootComponent->AddChild(mouseRotateComponent);

	mesh = CreateComponent<MeshComponent>("Mesh");
	mouseRotateComponent->AddChild(mesh);

	camera = CreateComponent<CameraComponent>("Camera");
	mouseRotateComponent->AddChild(camera);

	//@Todo: shield boundingbox is too large
	//shieldMesh = CreateComponent(MeshComponent("ico_sphere.vmesh", "shield.png"), "ShieldMesh");
	//rootComponent->AddChild(shieldMesh);
	//shieldMesh->SetActive(false);
}

void Player::Create()
{
	mesh->SetMeshFilename("jehuty.vmesh");

	camera->SetLocalPosition(1.f, 0.5f, -2.f);
	const XMVECTOR lookAtPoint = mesh->GetWorldPositionV() + (mesh->GetForwardVectorV() * 5.f);
	camera->SetWorldRotation(VMath::LookAtRotation(lookAtPoint, camera->GetWorldPositionV()));
}

void Player::Start()
{
	__super::Start();

	playerShieldWidget = UISystem::CreateWidget<PlayerShieldWidget>();
	playerShieldWidget->AddToViewport();

	reticleWidget = UISystem::CreateWidget<PlayerReticleWidget>();
	reticleWidget->AddToViewport();

	equippedGearsWidget = UISystem::CreateWidget<EquippedGearsWidget>();
	equippedGearsWidget->AddToViewport();

	SetCameraAsActive();

	SetEquippedGears();
}

void Player::End()
{
	previousHitTransparentActors.clear();
}

void Player::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	PrimaryGearAction();
	SecondaryGearAction();
	//ShieldLogic(deltaTime);

	LockOnTarget();

	Interact();

	LandToAirFormChange();

	MovementInput(deltaTime);
}

Properties Player::GetProps()
{
    auto props = __super::GetProps();
	props.title = "Player";
	props.Add("All Range Mode", &allRangeModeActive);
	return props;
}

void Player::InflictDamage(float damage)
{
	shields -= damage;
	shieldCountdownTimer = 0.5f;
}

XMVECTOR Player::GetAimDirection()
{
	const XMVECTOR cameraPos = camera->GetWorldPositionV();

	HitResult hit(this);
	if (Raycast(hit, cameraPos, camera->GetForwardVectorV(), 100.f))
	{
		const float hitDist = XMVector3Length(hit.GetHitPosV() - cameraPos).m128_f32[0];
		const float playerDist = XMVector3Length(GetPositionV() - cameraPos).m128_f32[0];

		if (hitDist < playerDist) //If hit object from camera to aim point is behind the player
		{
			return mouseRotateComponent->GetForwardVectorV();
		}

		return XMVector3Normalize(hit.GetHitPosV() - GetPositionV());
	}

	return mouseRotateComponent->GetForwardVectorV();
}

//Note: Default blend state needs to already be set for the mesh.
void Player::MakeOccludingMeshBetweenCameraAndPlayerTransparent()
{
	auto SetActorAlpha = [](Actor* actor, float alpha) {
		auto mesh = actor->GetFirstComponentOfTypeAllowNull<MeshComponent>();
		if (mesh && mesh->transparentOcclude)
		{
			auto ambientColour = mesh->GetAmbientColour();
			ambientColour.w = alpha;
			mesh->SetAmbientColour(ambientColour);
		}
	};

	const float transparentValue = 0.35f;
	const float solidValue = 1.f;

	HitResult hitResult(this);
	if (OrientedBoxCast(hitResult, camera->GetWorldPositionV(), GetPositionV(), XMFLOAT2(0.5f, 0.5f), true))
	{
		std::vector<Actor*> ableActors;

		for (auto actor : previousHitTransparentActors)
		{
			SetActorAlpha(actor, solidValue);
		}

		for (auto actor : hitResult.hitActors)
		{
			if (actor->CanBeTransparentlyOccluded())
			{
				ableActors.emplace_back(actor);
				SetActorAlpha(actor, transparentValue);
			}
		}

		previousHitTransparentActors = ableActors;
	}
	else
	{
		for(auto actor : previousHitTransparentActors)
		{
			SetActorAlpha(actor, solidValue);
		}

		previousHitTransparentActors.clear();
	}
}

void Player::MovementInput(float deltaTime)
{
	movementSpeed = std::clamp(movementSpeed, 0.f, 23.5f);

	const XMVECTOR offset = movementDirection * movementSpeed * deltaTime;
	XMFLOAT3 offsetFloat3;
	XMStoreFloat3(&offsetFloat3, offset);
	characterController->Move(offsetFloat3, deltaTime);

	if (isMoving)
	{
		movementSpeed += 65.f * deltaTime;
	}
	else
	{
		movementSpeed -= 87.5f * deltaTime;
	}

	SpatialComponent* moveComponent = nullptr;
	if (lockOnActor != nullptr)
	{
		moveComponent = camera;
	}
	else
	{
		moveComponent = mouseRotateComponent;
	}

	if (Input::GetKeyHeld(Keys::W))
	{
		movementDirection = moveComponent->GetForwardVectorV();
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::S))
	{
		movementDirection = -moveComponent->GetForwardVectorV();
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::A))
	{
		movementDirection = -moveComponent->GetRightVectorV();
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::D))
	{
		movementDirection = moveComponent->GetRightVectorV();
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::Q))
	{
		movementDirection = -moveComponent->GetUpVectorV();
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::E))
	{
		movementDirection = moveComponent->GetUpVectorV();
		isMoving = true;
	}

	if (Input::GetKeyHeld(Keys::W) && Input::GetKeyHeld(Keys::D))
	{
		movementDirection = XMVector3Normalize(moveComponent->GetForwardVectorV() + moveComponent->GetRightVectorV());
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::W) && Input::GetKeyHeld(Keys::A))
	{
		movementDirection = XMVector3Normalize(moveComponent->GetForwardVectorV() + -moveComponent->GetRightVectorV());
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::W) && Input::GetKeyHeld(Keys::Q))
	{
		movementDirection = XMVector3Normalize(moveComponent->GetForwardVectorV() + -moveComponent->GetUpVectorV());
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::W) && Input::GetKeyHeld(Keys::E))
	{
		movementDirection = XMVector3Normalize(moveComponent->GetForwardVectorV() + moveComponent->GetUpVectorV());
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::S) && Input::GetKeyHeld(Keys::A))
	{
		movementDirection = XMVector3Normalize(-moveComponent->GetForwardVectorV() + -moveComponent->GetRightVectorV());
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::S) && Input::GetKeyHeld(Keys::D))
	{
		movementDirection = XMVector3Normalize(-moveComponent->GetForwardVectorV() + moveComponent->GetRightVectorV());
		isMoving = true;
	}

	if (Input::GetKeyUp(Keys::W))
	{
		isMoving = false;
	}
	if (Input::GetKeyUp(Keys::A))
	{
		isMoving = false;
	}
	if (Input::GetKeyUp(Keys::S))
	{
		isMoving = false;
	}
	if (Input::GetKeyUp(Keys::D))
	{
		isMoving = false;
	}
	if (Input::GetKeyUp(Keys::Q))
	{
		isMoving = false;
	}
	if (Input::GetKeyUp(Keys::E))
	{
		isMoving = false;
	}
}

void Player::SetCameraAsActive()
{
	camera->SetAsActiveCamera();
	auto cameraFocusPoint = GetPositionV() + GetForwardVectorV() * 3.f;
	camera->SetWorldRotation(VMath::LookAtRotation(cameraFocusPoint, camera->GetWorldPositionV()));
}

void Player::SetMouseInputForCameraRotation(bool useInput)
{
	mouseRotateComponent->useMouseInput = useInput;
	UISystem::SetWidgetControlActive(!useInput);
}

void Player::PrimaryGearAction()
{
	auto inputType = primaryGear->GetInputType();
	switch (inputType)
	{
		case GearInputType::Press:
		{
			if (Input::GetMouseLeftUp())
			{
				primaryGear->Use();
			}
			break;
		}
		case GearInputType::Hold:
		{
			if (Input::GetMouseLeftDown())
			{
				primaryGear->Use();
			}
			break;
		}
	}
}

void Player::SecondaryGearAction()
{
	auto inputType = secondaryGear->GetInputType();
	switch (inputType)
	{
		case GearInputType::Press:
		{
			if (Input::GetMouseRightUp())
			{
				secondaryGear->Use();
			}
			break;
		}
		case GearInputType::Hold:
		{
			if (Input::GetMouseRightDown())
			{
				secondaryGear->Use();
			}
			break;
		}
	}
}

void Player::ShieldLogic(float deltaTime)
{
	if (shieldCountdownTimer > 0.f)
	{
		shieldCountdownTimer -= deltaTime;

		shieldMesh->SetActive(true);
		shieldMesh->AddLocalRotation(GetRightVectorV(), 500.f * deltaTime);
	}
	else
	{
		shieldMesh->SetActive(false);
	}
}

void Player::Interact()
{
	if (Input::GetKeyUp(Keys::Up))
	{
		HitResult result(this);
		if (Raycast(result, GetPositionV(), GetPositionV() + GetForwardVectorV() * 2.f))
		{
			auto interactActor = dynamic_cast<InteractActor*>(result.hitActor);
			if (interactActor)
			{
				interactActor->Interact();
			}
		}
	}
}

void Player::SetEquippedGears()
{
	primaryGear = GearSystem::Get().GetPrimaryGear();
	secondaryGear = GearSystem::Get().GetSecondaryGear();
}

void Player::LockOnTarget()
{
	if (Input::GetKeyUp(Keys::Space))
	{
		HitResult hit(this);
		if (Raycast(hit, GetPositionV(), GetForwardVectorV(), 50.f))
		{
			lockOnActor = hit.hitActor;
			camera->targetActor = lockOnActor;
		}
	}
}

void Player::LandToAirFormChange()
{
	if (Input::GetKeyUp(Keys::Shift))
	{
		if (characterController->GetGravityState())
		{
			characterController->SetGravityState(false);
		}
		else
		{
			characterController->SetGravityState(true);
		}
	}
}

#include "vpch.h"
#include "Player.h"
#include <algorithm>
#include "Core/Input.h"
#include "Core/VMath.h"
#include "Core/Timer.h"
#include "Actors/Game/Enemy.h"
#include "Actors/Game/LevelInstance.h"
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

	camera->SetAsActiveCamera();
	auto cameraFocusPoint = GetPositionV() + GetForwardVectorV() * 3.f;
	camera->SetWorldRotation(VMath::LookAtRotation(cameraFocusPoint, camera->GetWorldPositionV()));

	SetEquippedGears();
}

void Player::End()
{
	previousHitTransparentActors.clear();
}

void Player::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	SetReticleWidgetPosition(deltaTime);

	MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	PrimaryGearAction();
	SecondaryGearAction();
	//ShieldLogic(deltaTime);

	Interact();

	MovementInput(deltaTime);
	RotationInput();
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
	movementSpeed = std::clamp(movementSpeed, 0.f, 40.f);

	const XMVECTOR offset = movementDirection * movementSpeed * deltaTime;
	XMFLOAT3 offsetFloat3;
	XMStoreFloat3(&offsetFloat3, offset);
	characterController->Move(offsetFloat3, deltaTime);

	if (isMoving)
	{
		movementSpeed += 70.f * deltaTime;
	}
	else
	{
		movementSpeed -= 60.f * deltaTime;
	}

	if (Input::GetKeyHeld(Keys::W))
	{
		movementDirection = mouseRotateComponent->GetForwardVectorV();
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::S))
	{
		movementDirection = -mouseRotateComponent->GetForwardVectorV();
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::A))
	{
		movementDirection = -mouseRotateComponent->GetRightVectorV();
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::D))
	{
		movementDirection = mouseRotateComponent->GetRightVectorV();
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::Q))
	{
		movementDirection = -mouseRotateComponent->GetUpVectorV();
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::E))
	{
		movementDirection = mouseRotateComponent->GetUpVectorV();
		isMoving = true;
	}

	if (Input::GetKeyHeld(Keys::W) && Input::GetKeyHeld(Keys::D))
	{
		movementDirection = XMVector3Normalize(mouseRotateComponent->GetForwardVectorV() + mouseRotateComponent->GetRightVectorV());
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::W) && Input::GetKeyHeld(Keys::A))
	{
		movementDirection = XMVector3Normalize(mouseRotateComponent->GetForwardVectorV() + -mouseRotateComponent->GetRightVectorV());
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::W) && Input::GetKeyHeld(Keys::Q))
	{
		movementDirection = XMVector3Normalize(mouseRotateComponent->GetForwardVectorV() + -mouseRotateComponent->GetUpVectorV());
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::W) && Input::GetKeyHeld(Keys::E))
	{
		movementDirection = XMVector3Normalize(mouseRotateComponent->GetForwardVectorV() + mouseRotateComponent->GetUpVectorV());
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::S) && Input::GetKeyHeld(Keys::A))
	{
		movementDirection = XMVector3Normalize(-mouseRotateComponent->GetForwardVectorV() + -mouseRotateComponent->GetRightVectorV());
		isMoving = true;
	}
	else if (Input::GetKeyHeld(Keys::S) && Input::GetKeyHeld(Keys::D))
	{
		movementDirection = XMVector3Normalize(-mouseRotateComponent->GetForwardVectorV() + mouseRotateComponent->GetRightVectorV());
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

	if (CheckForObstacle())
	{
		movementDirection = XMVectorZero();
	}

	if (!CheckPlayerWithinLevelBounds())
	{
	}
}

void Player::RotationInput()
{
	if (Input::GetKeyHeld(Keys::Shift) && Input::GetKeyUp(Keys::Left))
	{
	}
	else if (Input::GetKeyHeld(Keys::Shift) && Input::GetKeyUp(Keys::Right))
	{
	}
	else if (Input::GetKeyHeld(Keys::Shift) && Input::GetKeyUp(Keys::Up))
	{
	}
	else if (Input::GetKeyHeld(Keys::Shift) && Input::GetKeyUp(Keys::Down))
	{
	}
	else if (Input::GetKeyUp(Keys::Right))
	{
	}
	else if (Input::GetKeyUp(Keys::Left))
	{
	}
}

bool Player::CheckPlayerWithinLevelBounds()
{
	return true;
}

bool Player::CheckForObstacle()
{
	return false;
}

void Player::PrimaryGearAction()
{
	if (Input::GetKeyDown(Keys::Up))
	{
		primaryGear->Use();
	}
}

void Player::SecondaryGearAction()
{
	if (Input::GetKeyDown(Keys::Down))
	{
		secondaryGear->Use();
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

//Map reticle to hit actor on raycast (think like Ocarina of Time Z-Target icon) or place just in front of player.
void Player::SetReticleWidgetPosition(float deltaTime)
{
	HitResult hit(this);
	if (Raycast(hit, camera->GetWorldPositionV(), camera->GetForwardVectorV(), 100.f))
	{
		reticleWidgetNextPos = hit.hitActor->GetHomogeneousPositionV();
		reticleWidgetLerpValue += deltaTime * 2.f;
	}
	else
	{
		reticleWidgetNextPos = GetHomogeneousPositionV();
		reticleWidgetLerpValue -= deltaTime * 2.f;
	}

	reticleWidgetLerpValue = std::clamp(reticleWidgetLerpValue, 0.f, 1.f);

	reticleWidget->worldPosition =
		XMVectorLerp(reticleWidget->worldPosition, reticleWidgetNextPos, reticleWidgetLerpValue);
}

void Player::SetEquippedGears()
{
	primaryGear = GearSystem::Get().GetPrimaryGear();
	secondaryGear = GearSystem::Get().GetSecondaryGear();
}

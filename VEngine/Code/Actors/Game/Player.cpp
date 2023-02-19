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
#include "Components/Lights/PointLightComponent.h"
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
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent = mesh;

	camera = CreateComponent<CameraComponent>("Camera");
	rootComponent->AddChild(camera);

	//@Todo: shield boundingbox is too large
	//shieldMesh = CreateComponent(MeshComponent("ico_sphere.vmesh", "shield.png"), "ShieldMesh");
	//rootComponent->AddChild(shieldMesh);
	//shieldMesh->SetActive(false);
}

void Player::Create()
{
	movementSpeed = 10.f;
	rotationSpeed = 10.f;

	mesh->SetMeshFilename("jehuty.vmesh");

	camera->SetLocalPosition(1.f, 0.5f, -2.f);
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

	nextPos = GetPositionV();
	nextRot = GetRotationV();

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

	MovementInput();
	RotationInput();

	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, movementSpeed));
	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, rotationSpeed));
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

void Player::MovementInput()
{
	if (!CheckMovementAndRotationHaveStopped()) return;

	auto previousPos = nextPos;

	if (Input::GetKeyHeld(Keys::W))
	{
		nextPos = GetPositionV() + GetForwardVectorV();
		if (CheckForObstacle()) nextPos = previousPos;
	}
	else if (Input::GetKeyHeld(Keys::S))
	{
		nextPos = GetPositionV() - GetForwardVectorV();
		if (CheckForObstacle()) nextPos = previousPos;
	}
	else if (Input::GetKeyHeld(Keys::A))
	{
		nextPos = GetPositionV() - GetRightVectorV();
		if (CheckForObstacle()) nextPos = previousPos;
	}
	else if (Input::GetKeyHeld(Keys::D))
	{
		nextPos = GetPositionV() + GetRightVectorV();
		if (CheckForObstacle()) nextPos = previousPos;
	}
	else if (Input::GetKeyHeld(Keys::E))
	{
		nextPos = GetPositionV() + GetUpVectorV();
		if (CheckForObstacle()) nextPos = previousPos;
	}
	else if (Input::GetKeyHeld(Keys::Q))
	{
		nextPos = GetPositionV() - GetUpVectorV();
		if (CheckForObstacle()) nextPos = previousPos;
	}

	if (!CheckPlayerWithinLevelBounds())
	{
		nextPos = previousPos;
	}
}

void Player::RotationInput()
{
	//Only check if rotation has stopped, as opposed to movement.
	if (!XMQuaternionEqual(GetRotationV(), nextRot)) return;

	if (Input::GetKeyHeld(Keys::Shift) && Input::GetKeyUp(Keys::Left))
	{
		nextRot = XMQuaternionMultiply(GetRotationV(), DirectX::XMQuaternionRotationAxis(GetForwardVectorV(), XMConvertToRadians(90.f)));
	}
	else if (Input::GetKeyHeld(Keys::Shift) && Input::GetKeyUp(Keys::Right))
	{
		nextRot = XMQuaternionMultiply(GetRotationV(), DirectX::XMQuaternionRotationAxis(GetForwardVectorV(), XMConvertToRadians(-90.f)));
	}
	else if (Input::GetKeyHeld(Keys::Shift) && Input::GetKeyUp(Keys::Up))
	{
		nextRot = XMQuaternionMultiply(GetRotationV(), DirectX::XMQuaternionRotationAxis(GetRightVectorV(), XMConvertToRadians(-90.f)));
	}
	else if (Input::GetKeyHeld(Keys::Shift) && Input::GetKeyUp(Keys::Down))
	{
		nextRot = XMQuaternionMultiply(GetRotationV(), DirectX::XMQuaternionRotationAxis(GetRightVectorV(), XMConvertToRadians(90.f)));
	}
	else if (Input::GetKeyUp(Keys::Right))
	{
		nextRot = XMQuaternionMultiply(GetRotationV(), DirectX::XMQuaternionRotationAxis(GetUpVectorV(), XMConvertToRadians(90.f)));
	}
	else if (Input::GetKeyUp(Keys::Left))
	{
		nextRot = XMQuaternionMultiply(GetRotationV(), DirectX::XMQuaternionRotationAxis(GetUpVectorV(), XMConvertToRadians(-90.f)));
	}
}

bool Player::CheckMovementAndRotationHaveStopped()
{
	return XMVector4Equal(GetPositionV(), nextPos) && XMQuaternionEqual(GetRotationV(), nextRot);
}

bool Player::CheckPlayerWithinLevelBounds()
{
	return LevelInstance::system.GetFirstActor()->CheckIfPointInsideLevelBounds(nextPos);
}

bool Player::CheckForObstacle()
{
	HitResult hitResult(this);
	return Raycast(hitResult, GetPositionV(), nextPos);
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

//@Todo: it's a bit hard to line up shots, even with the reticle. An idea might be to take from 
//Kirby on the Switch https://www.youtube.com/watch?v=RFbxi0faiec and do positional hit detection.
//Map reticle to hit actor on raycast (think like Ocarina of Time Z-Target icon) or place just in front of player.
void Player::SetReticleWidgetPosition(float deltaTime)
{
	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), GetForwardVectorV(), 100.f))
	{
		reticleWidgetNextPos = hit.hitActor->GetHomogeneousPositionV();
		reticleWidgetLerpValue += deltaTime * 2;
	}
	else
	{
		reticleWidgetNextPos = GetHomogeneousPositionV();
		reticleWidgetLerpValue -= deltaTime * 2;
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

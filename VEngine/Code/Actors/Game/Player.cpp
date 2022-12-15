#include "vpch.h"
#include "Player.h"
#include "Input.h"
#include "VMath.h"
#include "Components/MeshComponent.h"
#include "Components/CameraComponent.h"
#include "Physics/Raycast.h"

Player::Player()
{
	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh");
	rootComponent = mesh;

	camera = CreateComponent(CameraComponent(), "Camera");
	rootComponent->AddChild(camera);
}

void Player::Create()
{
	camera->SetPosition(2.f, 1.5f, -4.f);
}

void Player::Start()
{
	__super::Start();

	auto cameraFocusPoint = GetPositionV() + GetForwardVectorV() * 3.f;
	camera->SetWorldRotation(VMath::LookAtRotation(cameraFocusPoint, camera->GetWorldPositionV()));

	nextPos = GetPositionV();
	nextRot = GetRotationV();
}

void Player::End()
{
	previousHitTransparentActors.clear();
}

void Player::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	MovementInput();
	RotationInput();

	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, movementSpeed));
	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, rotationSpeed));
}

Properties Player::GetProps()
{
    auto props = __super::GetProps();
	props.title = "Player";
	return props;
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

	Ray ray(this);
	if (OrientedBoxCast(ray, camera->GetWorldPositionV(), GetPositionV(), XMFLOAT2(0.5f, 0.5f), true))
	{
		std::vector<Actor*> ableActors;

		for (auto actor : previousHitTransparentActors)
		{
			SetActorAlpha(actor, solidValue);
		}

		for (auto actor : ray.hitActors)
		{
			if (actor->CanBeTransparentlyOccluded())
			{
				ableActors.push_back(actor);
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

	if (Input::GetKeyHeld(Keys::W))
	{
		nextPos = GetPositionV() + GetForwardVectorV();
	}
	else if (Input::GetKeyHeld(Keys::S))
	{
		nextPos = GetPositionV() - GetForwardVectorV();
	}
	else if (Input::GetKeyHeld(Keys::A))
	{
		nextPos = GetPositionV() - GetRightVectorV();
	}
	else if (Input::GetKeyHeld(Keys::D))
	{
		nextPos = GetPositionV() + GetRightVectorV();
	}
}

void Player::RotationInput()
{
	if (!CheckMovementAndRotationHaveStopped()) return;

	if (Input::GetKeyDown(Keys::Right))
	{
		nextRot = XMQuaternionMultiply(GetRotationV(), DirectX::XMQuaternionRotationAxis(GetUpVectorV(), XMConvertToRadians(90.f)));
	}
	else if (Input::GetKeyDown(Keys::Left))
	{
		nextRot = XMQuaternionMultiply(GetRotationV(), DirectX::XMQuaternionRotationAxis(GetUpVectorV(), XMConvertToRadians(-90.f)));
	}
	else if (Input::GetKeyDown(Keys::Up))
	{
		nextRot = XMQuaternionMultiply(GetRotationV(), DirectX::XMQuaternionRotationAxis(GetRightVectorV(), XMConvertToRadians(-90.f)));
	}
	else if (Input::GetKeyDown(Keys::Down))
	{
		nextRot = XMQuaternionMultiply(GetRotationV(), DirectX::XMQuaternionRotationAxis(GetRightVectorV(), XMConvertToRadians(90.f)));
	}
}

bool Player::CheckMovementAndRotationHaveStopped()
{
	return XMVector4Equal(GetPositionV(), nextPos) && XMQuaternionEqual(GetRotationV(), nextRot);
}

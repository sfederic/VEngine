#include "vpch.h"
#include "ScannerEnemy.h"
#include "Physics/Raycast.h"
#include "Player.h"
#include "Core/VMath.h"
#include "Core/Core.h"
#include "Particle/Polyboard.h"

ScannerEnemy::ScannerEnemy()
{
	scanLaser = CreateComponent<Polyboard>("Laser");
	rootComponent->AddChild(scanLaser);
}

void ScannerEnemy::Create()
{
	movementPoints = 10;
	moveSpeed = 3.f;

	scanLaser->SetTextureFilename("Particle/beam_blue.png");
}

void ScannerEnemy::Tick(float deltaTime)
{
	ScanForPlayer();

	__super::Tick(deltaTime);

	//Careful with this call. It might be overriding NextRot sets in the parent tick.
	auto newRotation =
		XMQuaternionMultiply(nextRot,
			DirectX::XMQuaternionRotationAxis(VMath::GlobalUpVector(), XMConvertToRadians(deltaTime * 15.f)));
	SetNextRot(newRotation);
}

void ScannerEnemy::ScanForPlayer()
{
	const auto start = GetPositionV();

	scanLaser->SetStartPoint(start);

	switch (scanMode)
	{
	case ScanMode::Scanning:
	{
		const auto end = start + (GetForwardVectorV() * 5.f);
		scanLaser->SetEndPoint(end);

		HitResult hit(this);
		if (Raycast(hit, start, end))
		{
			auto player = hit.GetHitActorAs<Player>();
			if (player)
			{
				//Lock on to player
				scanMode = ScanMode::LockedOnToPlayer;
			}
		}

		RotateWhileScanning();

		break;
	}
	case ScanMode::LockedOnToPlayer:
	{
		const auto playerPos = Player::system.GetOnlyActor()->GetPositionV();

		scanLaser->SetEndPoint(playerPos);

		HitResult hit(this);
		if (Raycast(hit, start, playerPos))
		{
			auto player = hit.GetHitActorAs<Player>();
			if (!player)
			{
				//Can't find player
				scanMode = ScanMode::Scanning;
			}
		}

		SetRotation(VMath::LookAtRotation(playerPos, start));

		break;
	}
	}
}

//@Todo: ideally want two different scanner enemies. One that just spins, and one that scouts around a bit.
void ScannerEnemy::RotateWhileScanning()
{
	constexpr float scanInterval = 5.f;
	scanTimer += Core::GetDeltaTime();
	if (scanTimer > scanInterval)
	{
		scanTimer = 0.f;
		//MoveToRandomNode();
	}
}

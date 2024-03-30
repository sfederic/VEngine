#include "vpch.h"
#include "ScannerEnemy.h"
#include "Physics/Raycast.h"
#include "Player.h"
#include "Core/VMath.h"
#include "Particle/Polyboard.h"
#include "Gameplay/GameUtils.h"
#include "UI/Game/ScanningEnemyAlertWidget.h"
#include "UI/UISystem.h"

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

void ScannerEnemy::Start()
{
	__super::Start();

	alertWidget = UISystem::CreateWidget<ScanningEnemyAlertWidget>();
}

void ScannerEnemy::Tick(float deltaTime)
{
	__super::Tick(deltaTime);
	ScanForPlayer(deltaTime);
	alertWidget->SetWorldPosition(GetHomogeneousPositionV());
}

Properties ScannerEnemy::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Spin Speed", &spinSpeed);
	return props;
}

void ScannerEnemy::ScanForPlayer(float deltaTime)
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
		if (Physics::Raycast(hit, start, end))
		{
			scanLaser->SetEndPoint(hit.GetHitPosV());

			auto player = hit.GetHitActorAs<Player>();
			if (player)
			{
				//Lock on to player
				scanMode = ScanMode::LockedOnToPlayer;
				GameUtils::PlayAudioOneShot("alert.wav");
				alertWidget->AddToViewport(2.f);
			}
		}

		AddNextRot(VMath::GlobalUpVector(), spinSpeed);

		break;
	}
	case ScanMode::LockedOnToPlayer:
	{
		const auto playerPos = Player::system.GetOnlyActor()->GetPositionV();

		scanLaser->SetEndPoint(playerPos);

		HitResult hit(this);
		if (Physics::Raycast(hit, start, playerPos))
		{
			auto player = hit.GetHitActorAs<Player>();
			if (!player)
			{
				//Can't find player
				scanMode = ScanMode::Scanning;
				alertWidget->RemoveFromViewport();
			}
		}

		SetRotation(VMath::LookAtRotation(playerPos, start));

		break;
	}
	}
}

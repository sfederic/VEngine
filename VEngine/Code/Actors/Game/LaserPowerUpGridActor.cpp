#include "vpch.h"
#include "LaserPowerUpGridActor.h"
#include "Particle/Polyboard.h"
#include "Physics/Raycast.h"

LaserPowerUpGridActor::LaserPowerUpGridActor()
{
	laser = CreateComponent<Polyboard>("Laser");
	rootComponent->AddChild(laser);
}

void LaserPowerUpGridActor::Create()
{
	laser->SetTextureFilename("Particle/beam_blue.png");
}

void LaserPowerUpGridActor::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	laser->SetStartPoint(GetPositionV());

	PowerUpHitRaycastActor();
}

Properties LaserPowerUpGridActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void LaserPowerUpGridActor::PowerUpHitRaycastActor()
{
	const auto pos = GetPositionV();
	const auto end = pos + (GetForwardVectorV() * 10.f);
	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), end))
	{
		laser->SetEndPoint(hit.GetHitPosV());

		auto gridActor = hit.GetHitActorAs<GridActor>();
		if (gridActor)
		{
			previousHitGridActor = gridActor;
			gridActor->PowerUp();
		}

		if (hit.hitActor != previousHitGridActor)
		{
			PowerDownPreviousHitActor();
		}
	}
	else
	{
		laser->SetEndPoint(end);

		PowerDownPreviousHitActor();
	}
}

void LaserPowerUpGridActor::PowerDownPreviousHitActor()
{
	if (previousHitGridActor != nullptr)
	{
		previousHitGridActor->PowerDown();
		previousHitGridActor = nullptr;
	}
}

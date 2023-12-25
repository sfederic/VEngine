#include "vpch.h"
#include "LaserPowerUpGridActor.h"
#include "Particle/Polyboard.h"
#include "Physics/Raycast.h"

LaserPowerUpGridActor::LaserPowerUpGridActor()
{
	laser = CreateComponent<Polyboard>("Laser");
	rootComponent->AddChild(laser);
}

void LaserPowerUpGridActor::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	PowerUpHitRaycastActor();
	PowerDownPreviousHitActor();
}

Properties LaserPowerUpGridActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void LaserPowerUpGridActor::PowerUpHitRaycastActor()
{
	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), GetForwardVectorV(), 100.f))
	{
		auto gridActor = hit.GetHitActorAs<GridActor>();
		if (gridActor)
		{
			previousHitGridActor = gridActor;
			gridActor->PowerUp();
		}
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

#include "vpch.h"
#include "PowerUpLaserReflector.h"
#include "Particle/Polyboard.h"
#include "Physics/Raycast.h"

PowerUpLaserReflector::PowerUpLaserReflector()
{
	reflectedLaser = CreateComponent<Polyboard>("ReflectedLaser");
	rootComponent->AddChild(reflectedLaser);
}

void PowerUpLaserReflector::Create()
{
	__super::Create();

	reflectedLaser->SetTextureFilename("Particle/beam_blue.png");
}

void PowerUpLaserReflector::Start()
{
	__super::Start();

	reflectedLaser->SetActive(false);
}

void PowerUpLaserReflector::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	ReflectIncomingLaser();
}

void PowerUpLaserReflector::PowerUp()
{
	__super::PowerUp();

	reflectedLaser->SetActive(true);
}

void PowerUpLaserReflector::PowerDown()
{
	__super::PowerDown();

	reflectedLaser->SetActive(false);
}

void PowerUpLaserReflector::ReflectIncomingLaser()
{
	if (reflectedLaser->IsActive())
	{
		const auto pos = GetPositionV();
		reflectedLaser->SetStartPoint(pos);

		constexpr float range = 10.f;
		const auto newLaserEnd = pos + (GetRightVectorV() * range);
		reflectedLaser->SetEndPoint(newLaserEnd);
	}
}

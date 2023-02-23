#include "vpch.h"
#include "SwordBeam.h"

SwordBeam::SwordBeam()
{
	textureData.filename = "Particle/sword_slash.png";
}

void SwordBeam::Create()
{
	__super::Create();
}

void SwordBeam::Tick(float deltaTime)
{
	MoveAlongDirection(deltaTime);

	lifetime -= deltaTime;
	if (lifetime < 0.f)
	{
		Remove();
	}
}

void SwordBeam::MoveAlongDirection(float deltaTime)
{
	auto start = XMLoadFloat3(&startPoint);
	auto end = XMLoadFloat3(&endPoint);
	start += movementDirection * 20.f * deltaTime;
	end += movementDirection * 20.f * deltaTime;
	XMStoreFloat3(&startPoint, start);
	XMStoreFloat3(&endPoint, end);
}

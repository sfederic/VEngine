#include "ParticleEmitter.h"
#include "Billboard.h"
#include "Components/EmptyComponent.h"
#include "VMath.h"

ParticleEmitter::ParticleEmitter()
{
	rootComponent = EmptyComponent::system.Add(this);

	particleDirectionMin = XMFLOAT3(0.f, 0.f, 0.f);
	particleDirectionMax = XMFLOAT3(0.f, 0.f, 0.f);
}

void ParticleEmitter::Tick(float deltaTime)
{
	spawnTimer += deltaTime;

	if (spawnTimer > spawnRate)
	{
		auto transform = Transform();
		transform.position = GetPosition();
		transform.scale = GetScale();
		transform.rotation = GetRotation();

		auto bill = Billboard::system.Add(Billboard(), transform);
		activeBillboards.push_back(bill);

		float rangeX = VMath::RandomRange(particleDirectionMin.x, particleDirectionMax.x);
		float rangeY = VMath::RandomRange(particleDirectionMin.y, particleDirectionMax.y);
		float rangeZ = VMath::RandomRange(particleDirectionMin.z, particleDirectionMax.z);

		XMVECTOR direction = XMVectorSet(rangeX, rangeY, rangeZ, 0.f);
		bill->velocity = direction * (speed * deltaTime);

		spawnTimer = 0.f;
	}
}

Properties ParticleEmitter::GetProps()
{
	auto props = Actor::GetProps();
	props.Add("DirectionMin", &particleDirectionMin);
	props.Add("DirectionMax", &particleDirectionMax);
	props.Add("Rate", &spawnRate);
	props.Add("Speed", &speed);
	return props;
}

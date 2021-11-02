#include "ParticleEmitter.h"
#include "Billboard.h"
#include "Components/EmptyComponent.h"
#include "VMath.h"
#include "UI/SpriteBatcher.h"

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
		const float rangeX = VMath::RandomRange(particleDirectionMin.x, particleDirectionMax.x);
		const float rangeY = VMath::RandomRange(particleDirectionMin.y, particleDirectionMax.y);
		const float rangeZ = VMath::RandomRange(particleDirectionMin.z, particleDirectionMax.z);

		XMVECTOR direction = XMVectorSet(rangeX, rangeY, rangeZ, 0.f);

		auto transform = Transform();
		transform.position = GetPosition();
		transform.scale = GetScale();
		transform.rotation = GetRotation();

		Sprite sprite = {};
		sprite.dstRect = { 0, 0, 256, 256 };
		sprite.srcRect = { 0, 0, 563, 486};
		sprite.transform = transform;
		sprite.textureFilename = "bush.png";
		sprite.velocity = direction;
		spriteBatcher.CreateWorldSprite(sprite);

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

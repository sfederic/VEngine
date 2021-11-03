#pragma once
#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include <vector>
#include <string>
#include "Particle.h"

struct ParticleEmitter : SpatialComponent
{
	COMPONENT_SYSTEM(ParticleEmitter)

	std::vector<Particle> particles;

	XMFLOAT3 direction;

	std::string textureFilename;

	float speed = 1.0f;
	float lifetime = 0.5f;
	float spawnRate = 1.f;
	float spawnTimer = 0.f;

	ParticleEmitter();
	void CreateParticle(Particle particle);
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;
};

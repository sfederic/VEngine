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

	float moveSpeed = 1.0f;

	float lifetime = 1.f;
	float spawnRate = 0.2f;
	float spawnRadius = 0.f;

	float rotation = 0.f;
	float rotateSpeed = 0.f;

private:
	float spawnTimer = 0.f;

public:
	ParticleEmitter();
	void CreateParticle(Particle particle);
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;
};

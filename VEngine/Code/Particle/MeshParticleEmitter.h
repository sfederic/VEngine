#pragma once

#include "Actors/Actor.h"
#include "Actors/ActorSystem.h"
#include "Render/ShaderItem.h"
#include "ParticleData.h"
#include "Particle.h"

class InstanceMeshComponent;

class MeshParticleEmitter : public Actor
{
public:
	ACTOR_SYSTEM(MeshParticleEmitter);

	MeshParticleEmitter(std::string textureFilename = "test.png", ShaderItem* shaderItem = ShaderItems::Instance);
	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	ParticleData particleData;
	std::vector<Particle> particles;
	InstanceMeshComponent* instanceMesh = nullptr;

	float spawnTimer = 0.f;
};

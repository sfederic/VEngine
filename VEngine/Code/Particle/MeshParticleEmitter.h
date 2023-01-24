#pragma once

#include "Actors/Actor.h"
#include "Actors/ActorSystem.h"
#include "Render/ShaderItem.h"
#include "ParticleData.h"

class InstanceMeshComponent;

class MeshParticleEmitter : public Actor
{
public: 
	ACTOR_SYSTEM(MeshParticleEmitter);

	MeshParticleEmitter(std::string textureFilename = "test.png", ShaderItem* shaderItem = ShaderItems::Instance);

	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	ParticleData particleData;

	InstanceMeshComponent* instanceMesh = nullptr;

	float spawnTimer = 0.f;
};

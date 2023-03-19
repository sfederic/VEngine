#pragma once

#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Particle.h"
#include "Render/ShaderItem.h"
#include "ParticleData.h"

class Material;

class ParticleEmitter : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(ParticleEmitter)

	ParticleEmitter(std::string textureFilename = "test.png", ShaderItem* shaderItem = ShaderItems::DefaultClip);

	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;

	Material& GetMaterial() { return *material; }

	std::vector<Particle> particles;

	ParticleData particleData;

	//Separate from individual particle lifetime, Emitter destroys itself when it reaches a max.
	float emitterLifetime = 0.f;

private:
	Material* material = nullptr;

	float spawnTimer = 0.f;
	float emitterLifetimeTimer = 0.f;
};

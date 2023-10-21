#include "vpch.h"
#include "ParticleEmitter.h"
#include "Core/VMath.h"
#include "Render/Material.h"
#include "Render/MaterialSystem.h"

ParticleEmitter::ParticleEmitter(std::string textureFilename, ShaderItem* shaderItem)
{
	material = &MaterialSystem::CreateMaterial(textureFilename, shaderItem);
}

void ParticleEmitter::Tick(float deltaTime)
{
	spawnTimer += deltaTime;
	const float spawnRateRange = VMath::RandomRange(particleData.spawnRate.x, particleData.spawnRate.y);
	if (spawnTimer > spawnRateRange)
	{
		Particle particle = {};

		//Get the world position instead of relative
		XMMATRIX worldMatrix = this->GetWorldMatrix();
		XMStoreFloat3(&particle.transform.position, worldMatrix.r[3]);

		particle.SetParticleRangeData(particleData);

		particles.emplace_back(particle);

		spawnTimer = 0.f;
	}

	for (auto& particle : particles)
	{
		particle.lifetime += deltaTime;

		//Get random range between lifetimes
		float lifetimeRange = VMath::RandomRange(particleData.lifetime.x, particleData.lifetime.y);

		if (particle.lifetime > lifetimeRange)
		{
			std::swap(particle, particles.back());
			particles.pop_back();
		}

		particle.angle += particle.rotateSpeed * deltaTime;
		particle.AddVelocity(deltaTime);
	}

	emitterLifetimeTimer += deltaTime;
	if (emitterLifetime > 0.f && emitterLifetimeTimer > emitterLifetime)
	{
		this->Remove();
	}
}

void ParticleEmitter::Create()
{
	material->Create();
}

Properties ParticleEmitter::GetProps()
{
	auto props = __super::GetProps();

	auto particleDataProps = particleData.GetProps();
	props.Merge(particleDataProps);

	auto materialProps = material->GetProps();
	props.Merge(materialProps);

	props.title = "ParticleEmitter";

	return props;
}

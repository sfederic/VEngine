#include "vpch.h"
#include "MeshParticleEmitter.h"
#include "VMath.h"
#include "Components/InstanceMeshComponent.h"

constexpr int maxMeshes = 5;

MeshParticleEmitter::MeshParticleEmitter(std::string textureFilename, ShaderItem* shaderItem)
{
	instanceMesh = CreateComponent(InstanceMeshComponent(maxMeshes, "bevel_rock.vmesh", "test.png", shaderItem), "Mesh");
	rootComponent = instanceMesh;
}

void MeshParticleEmitter::Tick(float deltaTime)
{
	spawnTimer += deltaTime;
	const float spawnRateRange = VMath::RandomRange(particleData.spawnRate.x, particleData.spawnRate.y);
	if (spawnTimer > spawnRateRange && particles.size() < maxMeshes)
	{
		Particle particle;

		//Need to get the world position instead of relative
		const XMMATRIX worldMatrix = this->GetWorldMatrix();
		XMStoreFloat3(&particle.transform.position, worldMatrix.r[3]);

		particle.SetParticleRangeData(particleData);

		particles.emplace_back(particle);

		instanceMesh->instanceData[particles.size() - 1].world = worldMatrix;

		spawnTimer = 0.f;
	}

	for (int i = 0; i < particles.size(); i++)
	{
		auto& particle = particles[i];

		particle.lifetime += deltaTime;

		//Get random range between lifetimes
		const float lifetimeRange = VMath::RandomRange(particleData.lifetime.x, particleData.lifetime.y);

		if (particle.lifetime > lifetimeRange)
		{
			const int particlesSize = particles.size() - 1;
			std::swap(instanceMesh->instanceData[i], instanceMesh->instanceData[particlesSize]);
			instanceMesh->instanceData[particlesSize].world = VMath::ZeroMatrix();

			std::swap(particle, particles.back());
			particles.pop_back();
		}
		else
		{
			instanceMesh->instanceData[i].world.r[0].m128_f32[0] = 1.f;
			instanceMesh->instanceData[i].world.r[1].m128_f32[1] = 1.f;
			instanceMesh->instanceData[i].world.r[2].m128_f32[2] = 1.f;

			particle.AddVelocity(deltaTime);
			instanceMesh->instanceData[i].world.r[3] += XMLoadFloat3(&particle.direction) * (deltaTime * particle.moveSpeed);
		}
	}
}

Properties MeshParticleEmitter::GetProps()
{
	auto props = __super::GetProps();
	props.title = "MeshParticleEmitter";
	return props;
}

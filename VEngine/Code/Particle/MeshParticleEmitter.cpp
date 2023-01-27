#include "vpch.h"
#include "MeshParticleEmitter.h"
#include "VMath.h"
#include "Components/InstanceMeshComponent.h"

constexpr int maxMeshes = 5;

MeshParticleEmitter::MeshParticleEmitter(std::string textureFilename, ShaderItem* shaderItem)
{
	instanceMesh = CreateComponent(InstanceMeshComponent(maxMeshes, "cube.vmesh", "test.png", shaderItem), "Mesh");
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

		instanceMesh->instanceData[particles.size()].world = worldMatrix;

		//Set position from radius
		particle.transform.position.x += VMath::RandomRange(particleData.spawnRadius.x, particleData.spawnRadius.y);
		particle.transform.position.y += VMath::RandomRange(particleData.spawnRadius.x, particleData.spawnRadius.y);
		particle.transform.position.z += VMath::RandomRange(particleData.spawnRadius.x, particleData.spawnRadius.y);

		//Get random range between move speeds
		const float moveSpeedRange = VMath::RandomRange(particleData.moveSpeed.x, particleData.moveSpeed.y);
		particle.moveSpeed = moveSpeedRange;

		//Set particle rotation values
		const float rotateRange = VMath::RandomRange(particleData.rotation.x, particleData.rotation.y);
		particle.angle = rotateRange;

		const float rotateSpeedRange = VMath::RandomRange(particleData.rotateSpeed.x, particleData.rotateSpeed.y);
		particle.rotateSpeed = rotateSpeedRange;

		const XMFLOAT3 directionRange = VMath::RandomRangeFloat3(particleData.minDirection, particleData.maxDirection);
		particle.direction = directionRange;

		particles.emplace_back(particle);
		currentMeshIndex++;

		spawnTimer = 0.f;
	}

	for (int i = 0; i < particles.size(); i++)
	{
		auto& particle = particles[i];

		particle.lifetime += deltaTime;

		//Get random range between lifetimes
		float lifetimeRange = VMath::RandomRange(particleData.lifetime.x, particleData.lifetime.y);

		if (particle.lifetime > lifetimeRange)
		{
			std::swap(particle, particles.back());
			particles.pop_back();
			currentMeshIndex--;

			instanceMesh->instanceData[i].world = XMMatrixIdentity();
		}
		else
		{
			instanceMesh->instanceData[currentMeshIndex].world.r[3] += XMLoadFloat3(&particle.direction) * deltaTime * particle.moveSpeed;

			particle.AddVelocity(deltaTime);
		}
	}
}

Properties MeshParticleEmitter::GetProps()
{
	auto props = __super::GetProps();
	props.title = "MeshParticleEmitter";
	return props;
}

#include "vpch.h"
#include "MeshParticleEmitter.h"
#include "Core/VMath.h"
#include "Components/InstanceMeshComponent.h"

constexpr int maxMeshes = 5;

MeshParticleEmitter::MeshParticleEmitter(std::string textureFilename, ShaderItem* shaderItem)
{
	instanceMesh = CreateComponent("Mesh", InstanceMeshComponent(maxMeshes, "bevel_rock.vmesh", "test.png", shaderItem));
	rootComponent = instanceMesh;
}

void MeshParticleEmitter::Create()
{
	instanceMesh->ignoreGridRaycasts = true;
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

		instanceMesh->GetInstanceData()[particles.size() - 1].world = worldMatrix;

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
			std::swap(instanceMesh->GetInstanceData()[i], instanceMesh->GetInstanceData()[particlesSize]);
			instanceMesh->GetInstanceData()[particlesSize].world = VMath::ZeroMatrix();

			std::swap(particle, particles.back());
			particles.pop_back();
		}

		//Reset scale back to 1 so mesh is visible again after ZeroMatrix() calls
		instanceMesh->GetInstanceData()[i].world.r[0].m128_f32[0] = 1.f;
		instanceMesh->GetInstanceData()[i].world.r[1].m128_f32[1] = 1.f;
		instanceMesh->GetInstanceData()[i].world.r[2].m128_f32[2] = 1.f;

		XMVECTOR scale = XMLoadFloat3(&particle.transform.scale);
		XMVECTOR origin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		XMVECTOR translation = instanceMesh->GetInstanceData()[i].world.r[3] + XMLoadFloat3(&particle.direction) * (deltaTime * particle.moveSpeed);

		//@Todo: doesn't look great rotating on all 3 axis at once, but would need a property somewhere to denote
		//which axis only to increment.
		particle.yaw += particle.rotateSpeed * deltaTime;
		particle.roll += particle.rotateSpeed * deltaTime;
		particle.pitch += particle.rotateSpeed * deltaTime;
		XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(particle.pitch, particle.yaw, particle.roll);
		XMMATRIX T = XMMatrixAffineTransformation(scale, origin, rotation, translation);
		instanceMesh->GetInstanceData()[i].world = T;

		particle.angle += particle.rotateSpeed * deltaTime;

		particle.AddVelocity(deltaTime);
	}
}

Properties MeshParticleEmitter::GetProps()
{
	auto props = __super::GetProps();

	auto particleDataProps = particleData.GetProps();
	props.Merge(particleDataProps);

	props.title = "MeshParticleEmitter";
	return props;
}

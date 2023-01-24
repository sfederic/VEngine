#include "vpch.h"
#include "MeshParticleEmitter.h"
#include "VMath.h"
#include "Components/InstanceMeshComponent.h"
#include "Render/MaterialSystem.h"

MeshParticleEmitter::MeshParticleEmitter(std::string textureFilename, ShaderItem* shaderItem)
{
	material = MaterialSystem::CreateMaterial(textureFilename, shaderItem);

	instanceMesh = CreateComponent(InstanceMeshComponent(5, "cube.vmesh", "test.png", shaderItem), "Mesh");
	rootComponent = instanceMesh;
}

void MeshParticleEmitter::Tick(float deltaTime)
{
	/*spawnTimer += deltaTime;
	const float spawnRateRange = VMath::RandomRange(particleData.spawnRate.x, particleData.spawnRate.y);
	if (spawnTimer > spawnRateRange)
	{
		spawnTimer = 0.f;
	}*/

	for (auto& data : instanceMesh->instanceData)
	{
		data.world.r[3].m128_f32[2] += deltaTime;
	}
}

Properties MeshParticleEmitter::GetProps()
{
	auto props = __super::GetProps();
	props.title = "MeshParticleEmitter";
	return props;
}

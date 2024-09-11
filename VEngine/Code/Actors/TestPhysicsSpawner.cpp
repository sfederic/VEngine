#include "vpch.h"
#include "TestPhysicsSpawner.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"

TestPhysicsSpawner::TestPhysicsSpawner()
{
	meshSpawnTrigger = CreateComponent<BoxTriggerComponent>("Spawner");
	SetRootComponent(meshSpawnTrigger);
}

void TestPhysicsSpawner::Start()
{
	__super::Start();

	for (int i = 0; i < 10; i++)
	{
		const auto spawnPos = meshSpawnTrigger->GetRandomPointInTrigger();
		auto mesh = MeshComponent::system.Add(std::to_string(i), this);
		mesh->SetWorldPosition(spawnPos);
		mesh->SetMeshFilename("sphere.vmesh");
		mesh->SetWorldScale(0.35f);
		mesh->SetPhysicsStatic(false);
		mesh->Create();
	}
}

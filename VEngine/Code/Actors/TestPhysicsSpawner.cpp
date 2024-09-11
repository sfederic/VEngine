#include "vpch.h"
#include "TestPhysicsSpawner.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/VMath.h"

TestPhysicsSpawner::TestPhysicsSpawner()
{
	meshSpawnTrigger = CreateComponent<BoxTriggerComponent>("Spawner");
	AddChildToRoot(meshSpawnTrigger);
}

void TestPhysicsSpawner::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (!hasBeenEmptied && VMath::VecEqual(GetUpVectorV(), -VMath::GlobalUpVector()))
	{
		for (int i = 0; i < 20; i++)
		{
			const auto spawnPos = meshSpawnTrigger->GetRandomPointInTrigger();
			auto mesh = MeshComponent::system.Add(GetName() + "Mesh" + std::to_string(i), this);
			mesh->SetWorldPosition(spawnPos);
			mesh->SetMeshFilename("sphere.vmesh");
			mesh->SetWorldScale(0.1f);
			mesh->SetPhysicsStatic(false);
			mesh->SetPhysicsShape(PhysicsActorShape::Sphere);
			mesh->Create();
			mesh->ReCreateAsPhysicsActor(mesh->IsPhysicsStatic());
		}

		hasBeenEmptied = true;
	}
}

void TestPhysicsSpawner::End()
{
	for (auto mesh : GetComponents<MeshComponent>())
	{
		RemoveComponent(mesh);
		mesh->Remove();
	}
}

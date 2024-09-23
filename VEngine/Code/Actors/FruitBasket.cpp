#include "vpch.h"
#include "FruitBasket.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/VMath.h"

FruitBasket::FruitBasket()
{
	meshSpawnTrigger = CreateComponent<BoxTriggerComponent>("Spawner");
	AddChildToRoot(meshSpawnTrigger);
}

void FruitBasket::Tick(float deltaTime)
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

			physicsMeshes.push_back(mesh);
		}

		hasBeenEmptied = true;
	}
}

void FruitBasket::End()
{
	for (auto mesh : physicsMeshes)
	{
		RemoveComponent(mesh);
		mesh->Remove();
	}
}

Properties FruitBasket::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

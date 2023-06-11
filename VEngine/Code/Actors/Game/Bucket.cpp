#include "vpch.h"
#include "Bucket.h"
#include "Components/MeshComponent.h"
#include "WaterSource.h"
#include "FireSource.h"
#include "Core/VMath.h"
#include "Physics/Raycast.h"
#include "Gameplay/GameUtils.h"

Bucket::Bucket()
{
	waterMesh = CreateComponent<MeshComponent>("WaterMesh");
	rootComponent->AddChild(waterMesh);
}

void Bucket::Create()
{
	ignoreRotationValidCheck = true;

	mesh->SetMeshFilename("hollow_barrel.vmesh");

	waterMesh->SetMeshFilename("node.vmesh");
	waterMesh->SetTexture("water.jpg");
	waterMesh->SetLocalScale(0.75f, 0.75f, 0.75f);
	waterMesh->SetLocalPosition(0.f, 0.35f, 0.f);
}

void Bucket::Start()
{
	__super::Start();
}

void Bucket::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	waterMesh->SetVisibility(isFilled);

	if (CheckMovementAndRotationStopped())
	{
		CheckIfInWaterSource();
		EmptyWater();
	}
}

Properties Bucket::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("IsFilled", &isFilled);
	return props;
}

void Bucket::CheckIfInWaterSource()
{
	if (!isFilled)
	{
		if (VMath::VecEqual(GetUpVectorV(), VMath::GlobalUpVector()))
		{
			for (auto& waterSource : WaterSource::system.GetActors())
			{
				if (waterSource->Contains(GetPositionV()))
				{
					isFilled = true;
				}
			}
		}
	}
}

void Bucket::EmptyWater()
{
	if (isFilled)
	{
		if (!VMath::VecEqual(GetUpVectorV(), VMath::GlobalUpVector()))
		{
			isFilled = false;
			
			HitResult hit(this);
			const XMVECTOR raycastOrigin = GetPositionV() + GetUpVectorV();
			if (Raycast(hit, raycastOrigin, -VMath::GlobalUpVector(), 10.f))
			{
				auto gridActor = dynamic_cast<GridActor*>(hit.hitActor);
				if (gridActor)
				{
					gridActor->Douse();

					//Show a vapour effect if hit actor is a fire source
					auto fireSource = dynamic_cast<FireSource*>(hit.hitActor);
					if (fireSource)
					{
						GameUtils::SpawnParticleEmitter("Particle/smoke.png", fireSource->GetPositionV(), 1.5f);
					}
				}

				Transform t;
				t.position = hit.hitPos;
				t.position.y += 0.05f;
				auto puddle = MeshComponent::system.Add("", nullptr, MeshComponent("node.vmesh", "puddle.png"));
				puddle->Create();
				puddle->SetTransform(t);
			}
		}
	}
}

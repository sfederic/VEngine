#include "vpch.h"
#include "Bucket.h"
#include "Components/MeshComponent.h"
#include "WaterSource.h"
#include "Core/VMath.h"
#include "Physics/Raycast.h"

void Bucket::Create()
{
	mesh->SetMeshFilename("hollow_barrel.vmesh");
}

void Bucket::Start()
{
	__super::Start();
}

void Bucket::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	CheckIfInWaterSource();
	EmptyWater();
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
		if (VMath::VecEqual(GetUpVectorV(), -VMath::GlobalUpVector()))
		{
			isFilled = false;
			
			HitResult hit(this);
			if (Raycast(hit, GetPositionV(), -VMath::GlobalUpVector(), 100.f))
			{
				auto gridActor = dynamic_cast<GridActor*>(hit.hitActor);
				if (gridActor)
				{
					gridActor->Douse();
				}

				Transform t;
				t.position = hit.hitPos;
				t.position.y += 0.05f;
				auto puddle = MeshComponent::system.Add("", nullptr, MeshComponent("node.vmesh", "water.jpg"));
				puddle->Create();
				puddle->SetTransform(t);
			}
		}
	}
}

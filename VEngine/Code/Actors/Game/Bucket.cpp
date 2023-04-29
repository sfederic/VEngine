#include "vpch.h"
#include "Bucket.h"
#include "Components/MeshComponent.h"
#include "WaterSource.h"
#include "Core/VMath.h"

void Bucket::Create()
{
	mesh->SetMeshFilename("hollow_barrel.vmesh");
}

void Bucket::Tick(float deltaTime)
{
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
		if (XMVector4Equal(GetUpVectorV(), VMath::GlobalUpVector()))
		{
			for (auto& waterSource : WaterSource::system.GetActors())
			{
				waterSource->Contains(GetPositionV());
				isFilled = true;
			}
		}
	}
}

void Bucket::EmptyWater()
{
	if (isFilled)
	{
		if (XMVector4Equal(GetUpVectorV(), -VMath::GlobalUpVector()))
		{
			isFilled = false;
		}
	}
}

#include "vpch.h"
#include "Bucket.h"
#include "Components/MeshComponent.h"
#include "WaterSource.h"

void Bucket::Create()
{
	mesh->SetMeshFilename("hollow_barrel.vmesh");
}

void Bucket::Tick(float deltaTime)
{
	if (!isFilled)
	{
		for (auto& waterSource : WaterSource::system.GetActors())
		{
			waterSource->Contains(GetPositionV());
			isFilled = true;
		}
	}
}

Properties Bucket::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("IsFilled", &isFilled);
	return props;
}

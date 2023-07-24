#include "vpch.h"
#include "BucketPressurePlate.h"
#include "Components/BoxTriggerComponent.h"
#include "Bucket.h"

BucketPressurePlate::BucketPressurePlate()
{
	pressureTrigger = CreateComponent<BoxTriggerComponent>("PressureTrigger");
	rootComponent->AddChild(pressureTrigger);
}

void BucketPressurePlate::Create()
{
	SetMeshFilename("node.vmesh");

	pressureTrigger->SetLocalPosition(0.f, 0.5f, 0.f);

	DisableAllInteractivity();
}

void BucketPressurePlate::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	CheckBucketInPressureTrigger();
}

Properties BucketPressurePlate::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Plate Active", &pressurePlateActive);
	return props;
}

void BucketPressurePlate::CheckBucketInPressureTrigger()
{
	for (auto& bucket : Bucket::system.GetActors())
	{
		if (bucket->IsFilled())
		{
			if (pressureTrigger->Contains(bucket->GetPositionV()))
			{
				pressurePlateActive = true;
				return;
			}
		}
	}

	pressurePlateActive = false;
}

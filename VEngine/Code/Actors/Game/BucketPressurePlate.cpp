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

void BucketPressurePlate::Start()
{
	__super::Start();

	originalRestingPos = GetPositionV();
	emptyBucketPressurePos = GetPositionV() - (GetUpVectorV() * 0.2f);
	filledBucketPressurePos = GetPositionV() - (GetUpVectorV() * 0.5f);
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
		if (pressureTrigger->Contains(bucket->GetPositionV()))
		{
			if (bucket->IsFilled())
			{
				SetNextPos(filledBucketPressurePos);
				pressurePlateActive = true;
			}
			else
			{
				SetNextPos(emptyBucketPressurePos);
			}

			return;
		}
	}

	SetNextPos(originalRestingPos);
	pressurePlateActive = false;
}

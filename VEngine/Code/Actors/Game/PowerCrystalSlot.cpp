#include "vpch.h"
#include "PowerCrystalSlot.h"
#include "Physics/Raycast.h"
#include "PowerCrystal.h"

void PowerCrystalSlot::Create()
{
	__super::Create();

	SetMeshFilename("node.vmesh");
}

void PowerCrystalSlot::Start()
{
	__super::Start();

	powerUpActor = World::GetActorByNameAndLogCast<GridActor>(powerUpActorName);
}

void PowerCrystalSlot::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	HitResult hit(this);
	if (Physics::SimpleBoxCast(GetPositionV(), XMFLOAT3(0.5f, 0.5f, 0.5f), hit, true, true))
	{
		for (auto actor : hit.hitActors)
		{
			auto hitActor = dynamic_cast<PowerCrystal*>(actor);
			if (hitActor)
			{
				powerUpActor->PowerUp();
			}
		}
	}
	else
	{
		powerUpActor->PowerDown();
	}
}

Properties PowerCrystalSlot::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("PowerUp Actor", &powerUpActorName);
	return props;
}

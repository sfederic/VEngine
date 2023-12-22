#include "vpch.h"
#include "Lift.h"
#include "Components/BoxTriggerComponent.h"

Lift::Lift()
{
	liftTrigger = CreateComponent<BoxTriggerComponent>("LiftTrigger");
	rootComponent->AddChild(liftTrigger);
}

void Lift::Create()
{
	__super::Create();

	canFall = false;

	//Extents and offset are important here. If you stack up two 1x1x1 grid actors, then the lift will have
	//to move them both. Ideally, don't make levels where this is even possible, keep the lift singular.
	liftTrigger->SetExtents(0.45f, 1.f, 0.45f);
	liftTrigger->SetLocalPosition(0.f, 1.f, 0.f);

	SetMeshFilename("node.vmesh");
}

void Lift::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	for (auto gridActor : World::GetAllActorsAsBaseType<GridActor>())
	{
		if (liftTrigger->Contains(gridActor->GetPositionV()))
		{
			gridActor->SetCanFall(false);
		}
		else
		{
			gridActor->SetCanFall(true);
		}
	}
}

Properties Lift::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

std::vector<GridActor*> Lift::GetGridActorsContainedInLiftTrigger()
{
	std::vector<GridActor*> gridActors;
	for (auto gridActor : World::GetAllActorsAsBaseType<GridActor>())
	{
		if (gridActor == this) //Ignore self
		{
			continue;
		}
		if (liftTrigger->Contains(gridActor->GetPositionV()))
		{
			gridActors.emplace_back(gridActor);
		}
	}
	return gridActors;
}

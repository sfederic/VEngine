#include "vpch.h"
#include "Lift.h"
#include "Components/BoxTriggerComponent.h"

Lift::Lift()
{
	liftTrigger = CreateComponent<BoxTriggerComponent>("LiftTrigger");
	rootComponent->AddChild(liftTrigger);
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
		if (liftTrigger->Contains(gridActor->GetPositionV()))
		{
			gridActors.push_back(gridActor);
		}
	}
	return gridActors;
}

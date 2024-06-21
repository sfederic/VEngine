#include "vpch.h"
#include "PressureSwitch.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/GridActor.h"

PressureSwitch::PressureSwitch()
{
	switchTrigger = CreateComponent<BoxTriggerComponent>("SwitchTrigger");
	SetRootComponent(switchTrigger);

	switchMesh = CreateComponent<MeshComponent>("SwitchMesh");
	AddChildToRoot(switchMesh);
}

void PressureSwitch::Create()
{
	__super::Create();

	switchMesh->SetMeshFilename("pressure_switch.vmesh");
	switchMesh->SetLocalPosition(0.f, -0.5f, 0.f);
}

void PressureSwitch::Start()
{
	__super::Start();

	actorToActivate = World::GetActorByNameAllowNull(actorToActivateName);
}

void PressureSwitch::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (actorToActivate)
	{
		auto containedGridActors = switchTrigger->GetAllContainedActors<GridActor>();
		switchActive = containedGridActors.size() >= numOfActorsToActivateSwitch;

		if (switchActive)
		{
			actorToActivate->Activate();
		}
		else
		{
			actorToActivate->Deactivate();
		}
	}
}

Properties PressureSwitch::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Switch Active", &switchActive);
	props.Add("Actor To Activate", &actorToActivateName);
	props.Add("Num Actors Needed", &numOfActorsToActivateSwitch);
	return props;
}

#include "vpch.h"
#include "PressureSwitch.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/GridActor.h"

PressureSwitch::PressureSwitch()
{
	switchTrigger = CreateComponent<BoxTriggerComponent>("SwitchTrigger");
	rootComponent = switchTrigger;

	switchMesh = CreateComponent<MeshComponent>("SwitchMesh");
	rootComponent->AddChild(switchMesh);
}

void PressureSwitch::Create()
{
	__super::Create();

	switchMesh->SetMeshFilename("step_small.vmesh");
	switchMesh->SetLocalPosition(0.f, -0.5f, 0.f);
}

void PressureSwitch::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	auto containedGridActors = switchTrigger->GetAllContainedActors<GridActor>();
	switchActive = containedGridActors.size() > 0;
}

Properties PressureSwitch::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Switch Active", &switchActive);
	return props;
}

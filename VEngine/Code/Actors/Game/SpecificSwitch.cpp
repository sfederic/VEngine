#include "vpch.h"
#include "SpecificSwitch.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "GridActor.h"
#include "Core/Log.h"

SpecificSwitch::SpecificSwitch()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent = mesh;

	trigger = CreateComponent<BoxTriggerComponent>("Trigger");
	rootComponent->AddChild(trigger);
}

void SpecificSwitch::Create()
{
	mesh->SetMeshFilename("node.vmesh");
	mesh->SetAmbientColour(XMFLOAT4(1.f, 0.f, 0.f, 1.f));

	trigger->SetLocalPosition(0.f, 0.5f, 0.f);
}

void SpecificSwitch::Start()
{
	gridActorToCheck = dynamic_cast<GridActor*>(World::GetActorByNameAllowNull(gridActorToCheckName));
	if (gridActorToCheck == nullptr)
	{
		Log("%s not found for %s", gridActorToCheckName.c_str(), GetName().c_str());
	}
}

void SpecificSwitch::Tick(float deltaTime)
{
	CheckForSpecificGridActorOverlapping();
}

Properties SpecificSwitch::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("GridActor Name", &gridActorToCheckName);
	return props;
}

void SpecificSwitch::CheckForSpecificGridActorOverlapping()
{
	if (gridActorToCheck == nullptr) return;

	isActivated = trigger->Contains(gridActorToCheck->GetPositionV());

	if (isActivated)
	{
		mesh->SetAmbientColour(XMFLOAT4(0.f, 1.f, 0.f, 1.f));
	}
	else
	{
		mesh->SetAmbientColour(XMFLOAT4(1.f, 0.f, 0.f, 1.f));
	}
}

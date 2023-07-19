#include "vpch.h"
#include "MineCrane.h"
#include "Components/BoxTriggerComponent.h"

MineCrane::MineCrane()
{
	hookTrigger = CreateComponent<BoxTriggerComponent>("HookTrigger");
	rootComponent->AddChild(hookTrigger);
}

void MineCrane::Create()
{
	ignoreRotationValidCheck = true;
	canBeRotatedPitchXAxis = false;

	SetMeshFilename("crane.vmesh");

	hookTrigger->SetLocalPosition(0.f, 0.f, 2.f);
}

void MineCrane::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (hasActorHookedOn) return;
	
	for (auto gridActor : World::GetAllActorsAsBaseType<GridActor>())
	{
		if (hookTrigger->Contains(gridActor->GetPositionV()))
		{
			hasActorHookedOn = true;
		}
	}
}

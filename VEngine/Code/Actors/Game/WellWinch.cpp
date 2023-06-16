#include "vpch.h"
#include "WellWinch.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/VMath.h"

WellWinch::WellWinch()
{
	bucketTrigger = CreateComponent<BoxTriggerComponent>("BucketTrigger");
	rootComponent = bucketTrigger;

	rootComponent->AddChild(mesh);
}

void WellWinch::Create()
{
	canBeRotatedYawYAxis = false;

	mesh->SetMeshFilename("gear.vmesh");
	mesh->SetLocalPosition(1.f, 0.f, 0.f);
	mesh->AddLocalRotation(VMath::GlobalForwardVector(), 90.f);
}

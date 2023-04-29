#include "vpch.h"
#include "WaterSource.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "Render/RastStates.h"

WaterSource::WaterSource()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent = mesh;

	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent->AddChild(boxTrigger);
}

void WaterSource::Create()
{
	mesh->SetMeshFilename("plane.vmesh");
	mesh->SetRastState(RastStates::noBackCull);
}

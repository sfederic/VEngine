#include "vpch.h"
#include "PivotDoor.h"
#include "Components/MeshComponent.h"
#include "Render/RastStates.h"

PivotDoor::PivotDoor()
{
	SetEmptyRootComponent();
	rootComponent->AddChild(mesh);
}

void PivotDoor::Create()
{
	__super::Create();

	mesh->SetLocalPosition(-1.f, 0.f, 0.f);
	mesh->SetMeshFilename("plane.vmesh");
	mesh->SetRastState(RastStates::noBackCull);
}

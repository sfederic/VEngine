#include "vpch.h"
#include "PivotDoor.h"
#include "Components/MeshComponent.h"
#include "Render/RastStates.h"

PivotDoor::PivotDoor()
{
	SetEmptyRootComponent();
	rootComponent->AddChild(_mesh);
}

void PivotDoor::Create()
{
	__super::Create();

	_mesh->SetLocalPosition(0.5f, 0.25f, 0.f);
	_mesh->SetLocalScale(0.5f, 0.75f, 1.f);
	_mesh->SetMeshFilename("plane.vmesh");
	_mesh->SetRastState(RastStates::noBackCull);

	linkRotateAudio = "door.wav";
	rotateSpeed = 2.5f;

	canBeMovedInLink = false;

	canBeRotatedRollZAxis = false;
	canBeRotatedPitchXAxis = false;
	canBeRotatedYawYAxis = true;
}

Properties PivotDoor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

#include "vpch.h"
#include "FenceActor.h"
#include "Components/MeshComponent.h"
#include "Render/RastStates.h"

FenceActor::FenceActor()
{
	canBeRotatedInLink = false;
	canBeMovedInLink = false;
}

void FenceActor::Create()
{
	mesh->SetMeshFilename("plane.vmesh");
	mesh->SetTexture("fence01.png");
	mesh->SetRastState(RastStates::noBackCull);
}

Properties FenceActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

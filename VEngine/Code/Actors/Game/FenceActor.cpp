
#include "FenceActor.h"
import Components.MeshComponent;
#include "Render/RastStates.h"

FenceActor::FenceActor()
{
	canBeRotatedInLink = false;
	canBeMovedInLink = false;
}

void FenceActor::Create()
{
	_mesh->SetMeshFilename("plane.vmesh");
	_mesh->SetTexture("fence01.png");
	_mesh->SetRastState(RastStates::noBackCull);
}

Properties FenceActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

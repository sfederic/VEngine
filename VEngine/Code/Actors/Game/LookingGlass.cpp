#include "vpch.h"
#include "LookingGlass.h"
#include "Components/MeshComponent.h"
#include "Render/BlendStates.h"

void LookingGlass::Create()
{
	__super::Create();

	canFall = false;
	canBeRotatedPitchXAxis = false;

	SetMeshFilename("cube.vmesh");
	mesh->SetLocalScale(3.f, 3.f, 0.1f);
	mesh->SetBlendState(BlendStates::Default);
	mesh->SetAlpha(0.35f);
}

Properties LookingGlass::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

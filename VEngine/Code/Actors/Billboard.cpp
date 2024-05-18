#include "vpch.h"
#include "Billboard.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"
#include "Render/RastStates.h"

Billboard::Billboard()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent = mesh;
}

void Billboard::Create()
{
	__super::Create();

	mesh->SetMeshFilename("plane.vmesh");
	mesh->SetRastState(RastStates::noBackCull);
	mesh->SetShaderItem("Unlit");
}

void Billboard::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	auto transform = GetTransform();
	VMath::RotateTowardsCamera(transform);
	SetTransform(transform);
}

Properties Billboard::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

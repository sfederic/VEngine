
#include "Billboard.h"
import Components.MeshComponent;
import Core.VMath;
#include "Render/RastStates.h"

Billboard::Billboard()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	SetRootComponent(mesh);
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

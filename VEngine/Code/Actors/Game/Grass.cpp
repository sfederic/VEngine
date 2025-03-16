
#include "Grass.h"
import Components.MeshComponent;
#include "Render/RastStates.h"

Grass::Grass()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	SetRootComponent(mesh);
}

void Grass::Create()
{
	__super::Create();

	mesh->SetMeshFilename("plane.vmesh");
	mesh->SetTexture("Foliage/grass_blades.png");
	mesh->SetRastState(RastStates::noBackCull);
	mesh->SetShaderItem("Grass");
	mesh->SetWorldScale(0.5f);
}

Properties Grass::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

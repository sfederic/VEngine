
#include "SkyBox.h"
import Components.MeshComponent;
#include "Render/RastStates.h"

SkyBox::SkyBox()
{
	skyboxMesh = CreateComponent<MeshComponent>("SkyboxMesh");
	SetRootComponent(skyboxMesh);
}

void SkyBox::Create()
{
	__super::Create();

	skyboxMesh->skipPhysicsCreation = true;

	skyboxMesh->SetMeshFilename("sky_cylinder.vmesh");
	skyboxMesh->SetTexture("sky.jpg");;
	skyboxMesh->SetShaderItem("Unlit");
	skyboxMesh->SetWorldScale(XMVectorSet(50.f, 50.f, 50.f, 0.f));
	skyboxMesh->SetCollisionLayer(CollisionLayers::Editor);
	skyboxMesh->SetRastState(RastStates::noBackCull);
}

Properties SkyBox::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

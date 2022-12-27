#include "vpch.h"
#include "SkyBox.h"
#include "Components/MeshComponent.h"

SkyBox::SkyBox()
{
	auto mesh = CreateComponent(MeshComponent("sky_cylinder.fbx", "sky.jpg", ShaderItems::Unlit), "SkyBox Mesh");
	mesh->layer = CollisionLayers::Editor; //Make sure actors in-game can't interact with skyboxes' bounds.
	mesh->SetRastState(RastStates::noBackCull);

	rootComponent = mesh;
	SetScale(XMVectorSet(50.f, 50.f, 50.f, 0.f)); //@Todo: this setscale doesn't work. why?
}

Properties SkyBox::GetProps()
{
	auto props = __super::GetProps();
	props.title = "SkyBox";
	return props;
}

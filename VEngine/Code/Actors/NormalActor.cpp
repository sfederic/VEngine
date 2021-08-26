#include "NormalActor.h"
#include "Components/MeshComponent.h"
#include "Camera.h"

NormalActor::NormalActor()
{
	mesh1 = MeshComponent::system.Add(this, MeshComponent("cube.fbx"));
	rootComponent = mesh1;

	camera = CameraComponent::system.Add(this, CameraComponent(XMVectorSet(0.f, 0.f, -5.f, 1.f), false));
	camera->parent = mesh1;
}

Properties NormalActor::GetProps()
{
	Properties props = __super::GetProps();
	props.Add("MeshNum", &mesh1->number);
	return props;
}

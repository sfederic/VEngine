#include "NormalActor.h"
#include "Components/MeshComponent.h"

NormalActor::NormalActor()
{
	mesh1 = MeshComponent::system.Add(this, MeshComponent("cube.fbx"));
	mesh2 = MeshComponent::system.Add(this, MeshComponent("cube.fbx"));
	rootComponent = mesh1;
	mesh2->parent = mesh1;
	mesh2->transform.position.x += 3.0f;
}

Properties NormalActor::GetProps()
{
	Properties props = __super::GetProps();
	props.Add("MeshNum", &mesh1->number);
	return props;
}

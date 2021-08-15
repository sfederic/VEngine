#include "NormalActor.h"
#include "Components/MeshComponent.h"

NormalActor::NormalActor()
{
	mesh1 = MeshComponent::system.Add(this);
	mesh1->filename = "Meshes/monkey.fbx";
	rootComponent = mesh1;

	mesh2 = MeshComponent::system.Add(this);
	mesh2->filename = "Meshes/monkey.fbx";
	mesh2->transform.position.x += 3.0f;
	mesh1->AddChild(mesh2);
}

#include "NormalActor.h"
#include "Components/MeshComponent.h"

NormalActor::NormalActor()
{
	mesh1 = MeshComponent::system.Add(this);
	mesh1->filename = "Meshes/cube.fbx";
	rootComponent = mesh1;

	mesh2 = MeshComponent::system.Add(this);
	mesh2->filename = "Meshes/cube.fbx";
	mesh2->transform.position.x += 3.0f;
	mesh1->AddChild(mesh2);
}

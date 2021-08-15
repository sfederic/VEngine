#include "NormalActor.h"
#include "Components/MeshComponent.h"

NormalActor::NormalActor()
{
	mesh = MeshComponent::system.Add();
	mesh->filename = "Meshes/cube.fbx";
}

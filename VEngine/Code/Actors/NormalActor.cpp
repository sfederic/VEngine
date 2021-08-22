#include "NormalActor.h"
#include "Components/InstanceMeshComponent.h"

NormalActor::NormalActor()
{
	mesh1 = InstanceMeshComponent::system.Add(this, InstanceMeshComponent(1, "cube.fbx"));
	rootComponent = mesh1;
}

#include "vpch.h"
#include "DebugSphere.h"
#include "Components/MeshComponent.h"

DebugSphere::DebugSphere()
{
	sphereMesh = new MeshComponent("ico_sphere.vmesh", "test.png");
	sphereMesh->layer = CollisionLayers::Editor;
	sphereMesh->Create();
}

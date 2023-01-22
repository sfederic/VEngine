#include "vpch.h"
#include "DebugIcoSphere.h"
#include "Components/MeshComponent.h"

DebugIcoSphere::DebugIcoSphere()
{
	mesh = new MeshComponent("small_ico_sphere.vmesh", "test.png");
	mesh->layer = CollisionLayers::Editor;
	mesh->Create();
}

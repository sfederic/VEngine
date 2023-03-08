#include "vpch.h"
#include "DebugIcoSphere.h"
#include "Components/MeshComponent.h"

DebugIcoSphere::DebugIcoSphere()
{
	mesh = new MeshComponent("small_ico_sphere.vmesh", "test.png");
	mesh->SetCollisionLayer(CollisionLayers::Editor);
	mesh->Create();
}

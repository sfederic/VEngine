
#include "DebugSphere.h"
import Components.MeshComponent;

DebugSphere::DebugSphere()
{
	sphereMesh = new MeshComponent("ico_sphere.vmesh", "test.png");
	sphereMesh->SetCollisionLayer(CollisionLayers::Editor);
	sphereMesh->Create();
}

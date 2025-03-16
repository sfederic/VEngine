
#include "DebugIcoSphere.h"
import Components.MeshComponent;

DebugIcoSphere::DebugIcoSphere()
{
	mesh = new MeshComponent("small_ico_sphere.vmesh", "test.png");
	mesh->SetCollisionLayer(CollisionLayers::Editor);
	mesh->Create();
}

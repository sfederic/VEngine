
#include "DebugCone.h"
import Components.MeshComponent;

DebugCone::DebugCone()
{
	mesh = new MeshComponent("small_cone.vmesh", "test.png");
	mesh->SetCollisionLayer(CollisionLayers::Editor);
	mesh->Create();
}

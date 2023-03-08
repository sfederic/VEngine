#include "vpch.h"
#include "DebugCone.h"
#include "Components/MeshComponent.h"

DebugCone::DebugCone()
{
	mesh = new MeshComponent("small_cone.vmesh", "test.png");
	mesh->SetCollisionLayer(CollisionLayers::Editor);
	mesh->Create();
}

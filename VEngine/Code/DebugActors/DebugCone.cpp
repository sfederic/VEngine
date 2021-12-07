#include "DebugCone.h"
#include "Components/MeshComponent.h"

DebugCone::DebugCone()
{
	mesh = new MeshComponent("small_cone.fbx", "test.png");
	mesh->layer = CollisionLayers::Editor;
	mesh->Create();
}

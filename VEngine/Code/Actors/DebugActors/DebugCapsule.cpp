#include "vpch.h"
#include "DebugCapsule.h"
#include "Components/MeshComponent.h"

DebugCapsule::DebugCapsule()
{
	mesh = new MeshComponent("capsule.vmesh", "test.png");
	mesh->layer = CollisionLayers::Editor;
	mesh->SetRastState(RastStates::wireframe);
	mesh->Create();
}

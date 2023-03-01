#include "vpch.h"
#include "DebugCylinder.h"
#include "Components/MeshComponent.h"

DebugCylinder::DebugCylinder()
{
	mesh = new MeshComponent("cylinder.vmesh", "test.png");
	mesh->layer = CollisionLayers::Editor;
	mesh->SetRastState(RastStates::wireframe);
	mesh->Create();
}

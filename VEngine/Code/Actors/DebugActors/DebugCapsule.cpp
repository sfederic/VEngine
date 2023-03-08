#include "vpch.h"
#include "DebugCapsule.h"
#include "Components/MeshComponent.h"
#include "Render/RastStates.h"

DebugCapsule::DebugCapsule()
{
	mesh = new MeshComponent("capsule.vmesh", "test.png");
	mesh->SetCollisionLayer(CollisionLayers::Editor);
	mesh->SetRastState(RastStates::wireframe);
	mesh->Create();
}

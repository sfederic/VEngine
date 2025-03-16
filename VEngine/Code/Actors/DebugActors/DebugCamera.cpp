
#include "DebugCamera.h"
import Components.MeshComponent;

DebugCamera::DebugCamera()
{
	mesh = new MeshComponent("camera.vmesh", "test.png");
	mesh->SetCollisionLayer(CollisionLayers::Editor);
	mesh->Create();
}

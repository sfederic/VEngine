#include "vpch.h"
#include "DebugBox.h"
#include "Components/MeshComponent.h"

DebugBox::DebugBox()
{
	boxMesh = new MeshComponent("cube.vmesh", "test.png");
	boxMesh->SetCollisionLayer(CollisionLayers::Editor);
	boxMesh->Create();
}


#include "DebugBox.h"
import Components.MeshComponent;

DebugBox::DebugBox()
{
	boxMesh = new MeshComponent("cube.vmesh", "test.png");
	boxMesh->SetCollisionLayer(CollisionLayers::Editor);
	boxMesh->Create();
}

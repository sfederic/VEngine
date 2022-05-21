#include "vpch.h"
#include "DebugBox.h"
#include "Components/MeshComponent.h"

DebugBox::DebugBox()
{
	boxMesh = new MeshComponent("cube.fbx", "test.png");
	boxMesh->layer = CollisionLayers::Editor;
	boxMesh->Create();
}

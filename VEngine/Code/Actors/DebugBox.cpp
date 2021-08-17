#include "DebugBox.h"
#include "Components/MeshComponent.h"

DebugBox::DebugBox()
{
	boxMesh = new MeshComponent("cube.fbx");
	boxMesh->Create();
}

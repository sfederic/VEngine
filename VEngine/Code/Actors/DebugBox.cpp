#include "DebugBox.h"
#include "Components/MeshComponent.h"

DebugBox::DebugBox()
{
	boxMesh = MeshComponent::system.Add(nullptr, MeshComponent("cube.fbx"));
}

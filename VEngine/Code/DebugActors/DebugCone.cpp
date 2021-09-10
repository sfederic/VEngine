#include "DebugCone.h"
#include "Components/MeshComponent.h"

DebugCone::DebugCone()
{
	mesh = new MeshComponent("small_cone.fbx", L"test.png");
	mesh->Create();
}

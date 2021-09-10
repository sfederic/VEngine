#include "DebugCone.h"
#include "Components/MeshComponent.h"

DebugCone::DebugCone()
{
	mesh = new MeshComponent("cone.fbx", L"test.png");
	mesh->Create();
}

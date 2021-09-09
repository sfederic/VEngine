#include "DebugIcoSphere.h"
#include "Components/MeshComponent.h"

DebugIcoSphere::DebugIcoSphere()
{
	mesh = new MeshComponent("icosphere.fbx", L"test.png");
	mesh->Create();
}

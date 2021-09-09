#include "DebugSphere.h"
#include "Components/MeshComponent.h"

DebugSphere::DebugSphere()
{
	sphereMesh = new MeshComponent("sphere.fbx", L"test.png");
	sphereMesh->Create();
}

#include "DebugSphere.h"
#include "Components/MeshComponent.h"

DebugSphere::DebugSphere()
{
	sphereMesh = new MeshComponent("ico_sphere.fbx", L"test.png");
	sphereMesh->Create();
}

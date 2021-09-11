#include "DebugIcoSphere.h"
#include "Components/MeshComponent.h"

DebugIcoSphere::DebugIcoSphere()
{
	mesh = new MeshComponent("small_ico_sphere.fbx", L"test.png");
	mesh->Create();
}

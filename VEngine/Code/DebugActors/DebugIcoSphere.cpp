#include "DebugIcoSphere.h"
#include "Components/MeshComponent.h"

DebugIcoSphere::DebugIcoSphere()
{
	mesh = new MeshComponent("small_ico_sphere.fbx", "test.png");
	mesh->layer = CollisionLayers::Editor;
	mesh->Create();
}

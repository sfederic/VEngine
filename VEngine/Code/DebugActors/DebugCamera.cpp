#include "DebugCamera.h"
#include "Components/MeshComponent.h"

DebugCamera::DebugCamera()
{
	mesh = new MeshComponent("camera.fbx", "test.png");
	mesh->layer = Layer::Editor;
	mesh->Create();
}

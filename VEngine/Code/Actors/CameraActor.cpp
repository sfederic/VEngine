#include "vpch.h"
#include "CameraActor.h"
#include "Camera.h"

CameraActor::CameraActor()
{
	camera = CreateComponent(CameraComponent(), "Camera");
	rootComponent = camera;

	//Even though it's not an editor camera, there are really only player and editor cameras for now.
	//Set this and have this actor's camera work like staionary a editor camera.
	camera->isEditorCamera = true;
}

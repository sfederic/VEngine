#include "vpch.h"
#include "Camera.h"
#include "Actors/Game/Player.h"

static CameraComponent* activeCamera;
static EditorCamera editorCamera(XMFLOAT3(0.f, 1.f, -3.f));

CameraComponent& Camera::GetActiveCamera()
{
	return *activeCamera;
}

void Camera::SetActiveCamera(CameraComponent* camera)
{
	activeCamera = camera;
}

void Camera::SetEditorCameraAsActive()
{
	activeCamera = &editorCamera;
}

void Camera::SetActiveCameraTarget(Actor* newTarget)
{
	activeCamera->SetTargetActor(newTarget);
}

EditorCamera& Camera::GetEditorCamera()
{
	return editorCamera;
}

void Camera::SetActiveCameraTargetAndZoomIn(Actor* newTarget)
{
	GetActiveCamera().SetTargetActor(newTarget);
	Player::system.GetFirstActor()->SetZoomedInCameraFOV();
}

void Camera::SetActiveCameraTargetAndZoomOut(Actor* newTarget)
{
	GetActiveCamera().SetTargetActor(newTarget);
	Player::system.GetFirstActor()->SetDefaultCameraFOV();
}

void Camera::SetCameraBackToPlayer()
{
	auto player = Player::system.GetFirstActor();
	player->SetDefaultCameraFOV();
	GetActiveCamera().SetTargetActor(player);
}

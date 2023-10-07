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

EditorCamera& Camera::GetEditorCamera()
{
	return editorCamera;
}

void Camera::SetActiveCameraTarget(Actor* newTarget)
{
	Camera::GetActiveCamera().targetActor = newTarget;
}

void Camera::SetActiveCameraTargetAndZoomIn(Actor* newTarget)
{
	GetActiveCamera().targetActor = newTarget;
	Player::system.GetFirstActor()->SetZoomedInCameraFOV();
}

void Camera::SetActiveCameraTargetAndZoomOut(Actor* newTarget)
{
	GetActiveCamera().targetActor = newTarget;
	Player::system.GetFirstActor()->SetDefaultCameraFOV();
}

void Camera::SetCameraBackToPlayer()
{
	auto player = Player::system.GetFirstActor();
	player->SetDefaultCameraFOV();
	GetActiveCamera().targetActor = player;
}

void Camera::ShakeActiveCamera(float shakeLevel)
{
	activeCamera->shakeLevel = shakeLevel;
}

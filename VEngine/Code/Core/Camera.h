#pragma once

#include "EditorCamera.h"
#include "Components/CameraComponent.h"

class Actor;

namespace Camera
{
	CameraComponent& GetActiveCamera();
	EditorCamera& GetEditorCamera();
	void SetActiveCamera(CameraComponent* camera);
	void SetEditorCameraAsActive();
	void SetActiveCameraTarget(Actor* newTarget);
	void SetActiveCameraTargetAndZoomIn(Actor* newTarget);
	void SetActiveCameraTargetAndZoomOut(Actor* newTarget);
	void SetCameraBackToPlayer();
	void ShakeActiveCamera(float shakeLevel);
}

#pragma once

#include "Components/CameraComponent.h"

class Actor;
class EditorCamera;

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
}

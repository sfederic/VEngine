export Core.Camera;

export class Actor;
export class EditorCamera;
export class CameraComponent;

export namespace Camera
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

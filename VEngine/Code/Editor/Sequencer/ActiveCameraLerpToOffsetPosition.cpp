
#include "ActiveCameraLerpToOffsetPosition.h"
import Core.Camera;
import Core.VMath;
#include "Core/Log.h"

void ActiveCameraLerpToOffsetPosition::Tick(float deltaTime)
{
	auto& camera = Camera::GetActiveCamera();
	VMath::VectorConstantLerp(camera.GetWorldPositionV(),
		XMLoadFloat3(&positionToLerpTo), deltaTime, 5.f);
}

void ActiveCameraLerpToOffsetPosition::Activate()
{
	auto& camera = Camera::GetActiveCamera();
	originalCameraPosition = camera.GetWorldPosition();
}

void ActiveCameraLerpToOffsetPosition::Deactivate()
{
	auto& camera = Camera::GetActiveCamera();
	camera.SetWorldPosition(originalCameraPosition);
}

void ActiveCameraLerpToOffsetPosition::SetPositionToLerpTo()
{
	auto& camera = Camera::GetActiveCamera();
	positionToLerpTo = camera.GetWorldPosition();
	Log("Active camera snapshot position taken at x:%f y:%f z:%f",
		positionToLerpTo.x, positionToLerpTo.y, positionToLerpTo.z);
}

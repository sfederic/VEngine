#pragma once

#include "SpatialComponent.h"
#include "ComponentSystem.h"

class Actor;

class CameraComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(CameraComponent);

	CameraComponent() {}
	CameraComponent(XMFLOAT3 startPos);
	Properties GetProps() override;

	virtual XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();

	void SetAsActiveCamera();

	float GetCameraMoveSpeed() const { return cameraMoveSpeed; }
	void SetCameraMoveSpeed(float moveSpeed) { cameraMoveSpeed = moveSpeed; }
	float GetFOV() const { return FOV; }
	void SetFOV(float fov) { FOV = fov; }
	void SetShakeLevel(float shake) { shakeLevel = shake; }
	void SetTargetActor(Actor* actor) { targetActor = actor; }
	void SetTargetComponent(SpatialComponent* component) { targetComponent = component; }
	void SetLerpToFocusPoint(bool value);

	Actor& GetTargetActor() { return *targetActor; }

protected:
	XMVECTOR Shake();
	void ZoomTo(Actor* actor);
	void Move(float d, XMVECTOR axis);

	XMVECTOR focusPoint = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	Actor* targetActor = nullptr;
	SpatialComponent* targetComponent = nullptr;

	float shakeLevel = 0.f;
	float FOV = 60.f;
	float cameraMoveSpeed = 7.5f;

	float nearZ = 0.01f;
	float farZ = 1000.f;

	float lerpToFocusPointPercent = 0.f;
	bool lerpToFocusPoint = false;
};

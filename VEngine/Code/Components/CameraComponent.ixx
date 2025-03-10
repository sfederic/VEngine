#include <DirectXMath.h>

export module Components.CameraComponent;

import Components.SpatialComponent;
import Components.ComponentSystemDefine;
import Core.Properties;

export class Actor;

export class CameraComponent : public SpatialComponent, public ComponentSystemDefine<CameraComponent>
{
public:
	CameraComponent() {}
	CameraComponent(DirectX::XMFLOAT3 startPos);
	Properties GetProps() override;

	virtual DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetProjectionMatrix();

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
	DirectX::XMVECTOR Shake();
	void ZoomTo(Actor* actor);
	void Move(float d, DirectX::XMVECTOR axis);

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

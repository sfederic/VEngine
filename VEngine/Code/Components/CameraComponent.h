#pragma once

#include "SpatialComponent.h"
#include "ComponentSystem.h"

class Actor;

class CameraComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(CameraComponent)

	XMVECTOR focusPoint = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	Actor* targetActor = nullptr;

	float shakeLevel = 0.f;
	float FOV = 60.f;
	float cameraMoveSpeed = 7.5f;

private:
	float nearZ = 0.01f;
	float farZ = 1000.f;

public:
	CameraComponent();
	CameraComponent(XMFLOAT3 startPos);
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	virtual XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	void Pitch(float angle);
	void RotateY(float angle);
	void Move(float d, XMVECTOR axis);
	void ZoomTo(Actor* actor);
	XMVECTOR Shake();

private:
	void FPSCameraRotation();
};

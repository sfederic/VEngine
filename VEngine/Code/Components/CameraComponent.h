#pragma once

#include "SpatialComponent.h"
#include "ComponentSystem.h"

class Actor;

struct CameraComponent : SpatialComponent
{
	COMPONENT_SYSTEM(CameraComponent)

	XMVECTOR focusPoint = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	Actor* targetActor = nullptr;
	float shakeLevel = 0.f;
	float FOV = 60.f;
	float cameraMoveSpeed = 7.5f;

	CameraComponent() {}
	CameraComponent(XMFLOAT3 startPos);
	virtual XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	void Pitch(float angle);
	void RotateY(float angle);
	void Move(float d, XMVECTOR axis);
	void ZoomTo(Actor* actor);
	XMVECTOR Shake();

	virtual void Tick(float deltaTime) override {};
	virtual Properties GetProps() override;
	void FrustumCull();

private:
	float nearZ = 0.01f;
	float farZ = 1000.f;
};

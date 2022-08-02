#pragma once
#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include <DirectXMath.h>

using namespace DirectX;

class Actor;

struct CameraComponent : SpatialComponent
{
	COMPONENT_SYSTEM(CameraComponent)

	XMVECTOR focusPoint = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	Actor* targetActor = nullptr;

	float shakeLevel = 0.f;

	float FOV = 60.f;
	float nearZ = 0.01f;
	float farZ = 1000.f;
	float cameraMoveSpeed = 7.5f;

	bool isEditorCamera = false;
	bool arcBallMovementOn = false;

	CameraComponent() {}
	CameraComponent(XMFLOAT3 startPos, bool isEditorCamera_);
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	void Pitch(float angle);
	void RotateY(float angle);
	void MouseMove(int x, int y);
	void Move(float d, XMVECTOR axis);
	void ZoomTo(Actor* actor);
	XMVECTOR Shake();

	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;
	void FrustumCull();
};

extern CameraComponent editorCamera;
extern CameraComponent* activeCamera;

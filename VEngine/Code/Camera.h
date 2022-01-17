#pragma once
#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include <DirectXMath.h>

using namespace DirectX;

struct Actor;

//@Todo: need a camera shake for combat and whatever else

struct CameraComponent : SpatialComponent
{
	COMPONENT_SYSTEM(CameraComponent)

	XMVECTOR focusPoint;

	Actor* targetActor = nullptr;

	float FOV = 60.f;
	float nearZ = 0.01f;
	float farZ = 1000.f;

	bool editorCamera = false;
	bool arcBallMovementOn = false;

	CameraComponent(XMFLOAT3 startPos, bool isEditorCamera);
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	void Pitch(float angle);
	void RotateY(float angle);
	void MouseMove(int x, int y);
	void Move(float d, XMVECTOR axis);
	void ZoomTo(Actor* actor);

	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;
	void FrustumCull();
};

extern CameraComponent editorCamera;
extern CameraComponent* activeCamera;

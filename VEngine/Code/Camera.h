#pragma once
#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include <DirectXMath.h>

using namespace DirectX;

struct Actor;

//TODO: camera's transform variables aren't being updated, only world matrix is

struct CameraComponent : SpatialComponent
{
	COMPONENT_SYSTEM(CameraComponent)

	XMVECTOR focusPoint;
	Actor* targetActor = nullptr;
	bool editorCamera = false;

	float FOV = XM_PI / 3.f;
	float nearZ = 0.01f;
	float farZ = 1000.f;

	CameraComponent(XMFLOAT3 startPos, bool isEditorCamera);
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	void Pitch(float angle);
	void RotateY(float angle);
	void MouseMove(int x, int y);
	void Move(float d, XMVECTOR axis);
	void ZoomTo(Actor* actor);

	virtual void Tick(double deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;
	void FrustumCull();
};

extern CameraComponent editorCamera;
extern CameraComponent* activeCamera;

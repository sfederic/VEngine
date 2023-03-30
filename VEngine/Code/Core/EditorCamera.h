#pragma once

#include "Components/CameraComponent.h"

class EditorCamera : public CameraComponent
{
public:
	EditorCamera(XMFLOAT3 startPos) : CameraComponent(startPos) {}
	virtual void Tick(float deltaTime) override;
	XMMATRIX GetViewMatrix();

private:
	void MouseMove(int x, int y);

	XMINT2 lastMousePos;
	bool arcBallMovementOn = false;
};

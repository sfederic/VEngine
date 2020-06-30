#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera(XMVECTOR initialLocation); //Set axis to default LH

	void UpdateViewMatrix();
	void Strafe(float d);
	void MoveForward(float d);
	void MoveUp(float d);
	void Pitch(float angle);
	void RotateY(float angle);
	void MouseMove(int x, int y);

	XMVECTOR location;
	XMVECTOR focusPoint;
	XMVECTOR worldUp;

	XMVECTOR up, right, forward;

	XMMATRIX view;
};

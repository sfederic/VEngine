#include "Camera.h"
#include "Win32Util.h"

Camera::Camera(XMVECTOR initialLocation)
{
	forward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	right = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	worldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	focusPoint = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	location = initialLocation;

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	forward = XMVector3Normalize(forward);
	up = XMVector3Normalize(XMVector3Cross(forward, right));
	right = XMVector3Cross(up, forward);

	float x = XMVectorGetX(XMVector3Dot(location, right));
	float y = XMVectorGetX(XMVector3Dot(location, up));
	float z = XMVectorGetX(XMVector3Dot(location, forward));

	view.r[0].m128_f32[0] = right.m128_f32[0];
	view.r[1].m128_f32[0] = right.m128_f32[1];
	view.r[2].m128_f32[0] = right.m128_f32[2];
	view.r[3].m128_f32[0] = -x;

	view.r[0].m128_f32[1] = up.m128_f32[0];
	view.r[1].m128_f32[1] = up.m128_f32[1];
	view.r[2].m128_f32[1] = up.m128_f32[2];
	view.r[3].m128_f32[1] = -y;

	view.r[0].m128_f32[2] = forward.m128_f32[0];
	view.r[1].m128_f32[2] = forward.m128_f32[1];
	view.r[2].m128_f32[2] = forward.m128_f32[2];
	view.r[3].m128_f32[2] = -z;

	view.r[0].m128_f32[3] = 0.0f;
	view.r[1].m128_f32[3] = 0.0f;
	view.r[2].m128_f32[3] = 0.0f;
	view.r[3].m128_f32[3] = 1.0f;
}

//TODO: move all camera movement code to Actor
void Camera::Strafe(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	location = XMVectorMultiplyAdd(s, right, location);
}

void Camera::MoveForward(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	location = XMVectorMultiplyAdd(s, forward, location);	
}

void Camera::MoveUp(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	location = XMVectorMultiplyAdd(s, up, location);
}

void Camera::Pitch(float angle)
{
	XMMATRIX r = XMMatrixRotationAxis(right, angle);
	up = XMVector3TransformNormal(up, r);
	forward = XMVector3TransformNormal(forward, r);
}

void Camera::RotateY(float angle)
{
	XMMATRIX r = XMMatrixRotationY(angle);
	up = XMVector3TransformNormal(up, r);
	right = XMVector3TransformNormal(right, r);
	forward = XMVector3TransformNormal(forward, r);
}

void Camera::MouseMove(int x, int y)
{
	static POINT lastMousePos;

	if (GetAsyncKeyState(VK_RBUTTON) < 0)
	{
		SetCapture(mainWindow);

		float dx = XMConvertToRadians(0.25f * (float)(x - lastMousePos.x));
		float dy = XMConvertToRadians(0.25f * (float)(y - lastMousePos.y));

		Pitch(dy);
		RotateY(dx);

		ReleaseCapture();
	}

	lastMousePos.x = x;
	lastMousePos.y = y;
}
#include "Camera.h"
#include "CoreSystem.h"
#include "Actor.h"
#include "UISystem.h"
#include <omp.h>
#include "Input.h"
#include "Console.h"

Camera editorCamera(XMVectorSet(0.f, 0.f, -5.f, 1.f));

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

void Camera::Tick(float deltaTime)
{
	if (actorAttachedTo)
	{
		location = actorAttachedTo->GetPositionVector();
		location += attachedOffset;
	}

	MouseMove(gUISystem.mousePos.x, gUISystem.mousePos.y);
	UpdateViewMatrix();

	//WASD MOVEMENT
	if (!gConsole.bConsoleActive)
	{
		const float moveSpeed = 7.5f * deltaTime;

		if (GetAsyncKeyState('W'))
		{
			Move(moveSpeed, forward);
		}
		if (GetAsyncKeyState('S'))
		{
			Move(-moveSpeed, forward);
		}
		if (GetAsyncKeyState('D'))
		{
			Move(moveSpeed, right);
		}
		if (GetAsyncKeyState('A'))
		{
			Move(-moveSpeed, right);
		}
		if (GetAsyncKeyState('Q'))
		{
			Move(-moveSpeed, up);
		}
		if (GetAsyncKeyState('E'))
		{
			Move(moveSpeed, up);
		}

		//MOUSE WHEEL ZOOM
		const float zoomSpeed = 65.f * deltaTime;

		if (inputSystem.GetMouseWheelUp())
		{
			Move(zoomSpeed, editorCamera.forward);
		}
		else if (inputSystem.GetMouseWheelDown())
		{
			Move(-zoomSpeed, editorCamera.forward);
		}
	}
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
		SetCapture(gCoreSystem.mainWindow);

		float dx = XMConvertToRadians(0.25f * (float)(x - lastMousePos.x));
		float dy = XMConvertToRadians(0.25f * (float)(y - lastMousePos.y));

		Pitch(dy);
		RotateY(dx);

		ReleaseCapture();
	}

	lastMousePos.x = x;
	lastMousePos.y = y;
}

void Camera::Move(float d, XMVECTOR axis)
{
	XMVECTOR s = XMVectorReplicate(d);
	location = XMVectorMultiplyAdd(s, axis, location);
}

void Camera::FrustumCullTest(ActorSystem& system)
{
	#pragma omp parallel for
	for (int i = 0; i < system.actors.size(); i++)
	{
		XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);

		XMMATRIX world = system.actors[i].transform;
		XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(world), world);

		XMMATRIX viewToLocal = XMMatrixMultiply(invView, invWorld);

		BoundingFrustum frustum, localSpaceFrustum;
		BoundingFrustum::CreateFromMatrix(frustum, proj);
		frustum.Transform(localSpaceFrustum, viewToLocal);

		system.boundingBox.Center = system.actors[i].GetPositionFloat3();
		system.boundingBox.Extents = system.actors[i].GetScale();

		if (localSpaceFrustum.Contains(system.boundingBox) == DirectX::DISJOINT)
		{
			system.actors[i].bRender = false;
		}
		else
		{
			system.actors[i].bRender = true;
		}
	}
}

void Camera::AttachTo(Actor* actor)
{
	actorAttachedTo = actor;
	attachedOffset = this->location - actor->GetPositionVector();

	location = actor->GetPositionVector();
	location += attachedOffset;
}

Camera* GetPlayerCamera()
{
	return &editorCamera;
}

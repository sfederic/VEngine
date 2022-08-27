#include "vpch.h"
#include "EditorCamera.h"
#include "WorldEditor.h"
#include "VMath.h"
#include "Input.h"
#include "Editor/DebugMenu.h"
#include "Editor/Console.h"
#include "Editor/Editor.h"

void EditorCamera::Tick(float deltaTime)
{
	XMVECTOR right = GetRightVectorV();
	XMVECTOR up = GetUpVectorV();
	XMVECTOR forward = GetForwardVectorV();

	MouseMove(editor->viewportMouseX, editor->viewportMouseY);

	//WASD MOVEMENT
	if (!Console::bConsoleActive)
	{
		if (Input::GetMouseRightDown())
		{
			const float moveSpeed = cameraMoveSpeed * deltaTime;

			if (Input::GetKeyDown(Keys::W))
			{
				Move(moveSpeed, forward);
			}
			if (Input::GetKeyDown(Keys::S))
			{
				Move(-moveSpeed, forward);
			}
			if (Input::GetKeyDown(Keys::D))
			{
				Move(moveSpeed, right);
			}
			if (Input::GetKeyDown(Keys::A))
			{
				Move(-moveSpeed, right);
			}
			if (Input::GetKeyDown(Keys::Q))
			{
				Move(-moveSpeed, VMath::XMVectorUp());
			}
			if (Input::GetKeyDown(Keys::E))
			{
				Move(moveSpeed, VMath::XMVectorUp());
			}
		}

		//Zoom onto selected actor
		if (Input::GetKeyUp(Keys::F))
		{
			if (WorldEditor::GetPickedActor())
			{
				ZoomTo(WorldEditor::GetPickedActor());
			}
		}

		//MOUSE WHEEL ZOOM
		const float zoomSpeed = cameraMoveSpeed * 6.f * deltaTime;

		if (!debugMenu.hasMouseFocus) //Don't allow mousescroll camera zoom if Imgui debug menu is open
		{
			if (Input::mouseWheelUp)
			{
				Move(zoomSpeed, forward);
			}
			else if (Input::mouseWheelDown)
			{
				Move(-zoomSpeed, forward);
			}
		}
	}
}

XMMATRIX EditorCamera::GetViewMatrix()
{
	XMVECTOR forward = GetForwardVectorV();
	XMVECTOR position = GetPositionV();

	XMMATRIX view = XMMatrixIdentity();

	XMVECTOR focus = position + forward;

	if (arcBallMovementOn && WorldEditor::GetPickedActor())
	{
		focus = WorldEditor::GetPickedActor()->GetPositionVector();
	}

	view = XMMatrixLookAtLH(position, focus, VMath::XMVectorUp());
	return view;
}

void EditorCamera::MouseMove(int x, int y)
{
	static XMINT2 lastMousePos;

	const float dx = XMConvertToRadians(0.25f * (float)(x - lastMousePos.x));
	const float dy = XMConvertToRadians(0.25f * (float)(y - lastMousePos.y));

	arcBallMovementOn = false;

	if (Input::GetMouseRightDown() && Input::GetKeyDown(Keys::Ctrl))
	{
		//Arcball camera movement
		//REF:https://asliceofrendering.com/camera/2019/11/30/ArcballCamera/
		//@Todo: sort of works, but keeps reseting the camera's lookat direction when you release the arcball keys
		//because the rotation isn't updated below, only the position.

		arcBallMovementOn = true;

		auto pos = GetPositionV();

		Actor* pickedActor = WorldEditor::GetPickedActor();
		if (pickedActor == nullptr) return;

		auto pivot = pickedActor->GetPositionVector();

		focusPoint = pivot;

		XMMATRIX xRot = XMMatrixIdentity();
		xRot = XMMatrixRotationX(dy);
		pos = XMVector3Transform(pos - pivot, xRot) + pivot;

		XMMATRIX yRot = XMMatrixIdentity();
		yRot = XMMatrixRotationY(dx);
		pos = XMVector3Transform(pos - pivot, yRot) + pivot;

		SetPosition(pos);
	}
	else if (Input::GetMouseRightDown())
	{
		Pitch(dy);
		RotateY(dx);
	}

	lastMousePos.x = x;
	lastMousePos.y = y;
}

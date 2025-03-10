module;

#include "vpch.h"
#include "Components/CameraComponent.h"
#include "WorldEditor.h"
#include "VMath.h"
#include "Input.h"
#include "Editor/DebugMenu.h"
#include "Editor/Console.h"
#include "Editor/Editor.h"

export module Core.EditorCamera;

export class EditorCamera : public CameraComponent
{
public:
	EditorCamera(XMFLOAT3 startPos) : CameraComponent(startPos) {}
	virtual void Tick(float deltaTime) override;
	XMMATRIX GetViewMatrix();

private:
	void MouseMove(int x, int y);

	XMINT2 lastMousePos{};
	bool arcBallMovementOn = false;
};

void EditorCamera::Tick(float deltaTime)
{
	XMVECTOR right = GetRightVectorV();
	XMVECTOR up = GetUpVectorV();
	XMVECTOR forward = GetForwardVectorV();

	MouseMove(Editor::Get().GetViewportMouseX(), Editor::Get().GetViewportMouseY());

	//WASD MOVEMENT
	if (!Console::bConsoleActive)
	{
		if (Input::GetMouseRightDown())
		{
			const float moveSpeed = cameraMoveSpeed * deltaTime;

			if (Input::GetKeyHeld(Keys::W))
			{
				Move(moveSpeed, forward);
			}
			if (Input::GetKeyHeld(Keys::S))
			{
				Move(-moveSpeed, forward);
			}
			if (Input::GetKeyHeld(Keys::D))
			{
				Move(moveSpeed, right);
			}
			if (Input::GetKeyHeld(Keys::A))
			{
				Move(-moveSpeed, right);
			}
			if (Input::GetKeyHeld(Keys::Q))
			{
				Move(-moveSpeed, VMath::GlobalUpVector());
			}
			if (Input::GetKeyHeld(Keys::E))
			{
				Move(moveSpeed, VMath::GlobalUpVector());
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
			if (Input::IsMouseWheelUp())
			{
				Move(zoomSpeed, forward);
			}
			else if (Input::IsMouseWheelDown())
			{
				Move(-zoomSpeed, forward);
			}
		}
	}
}

XMMATRIX EditorCamera::GetViewMatrix()
{
	const auto worldMatrix = GetWorldMatrix();
	const auto pos = GetWorldPositionV();
	auto focus = pos + GetForwardVectorV();
	if (arcBallMovementOn && WorldEditor::GetPickedActor())
	{
		focus = WorldEditor::GetPickedActor()->GetPositionV();
	}
	return DirectX::XMMatrixLookAtLH(pos, focus, VMath::GlobalUpVector());
}

void EditorCamera::MouseMove(int x, int y)
{
	const float dx = XMConvertToRadians(0.25f * (float)(x - lastMousePos.x));
	const float dy = XMConvertToRadians(0.25f * (float)(y - lastMousePos.y));

	arcBallMovementOn = false;

	if (Input::GetMouseRightDown() && Input::GetKeyHeld(Keys::Ctrl))
	{
		//Arcball camera movement
		//REF:https://asliceofrendering.com/camera/2019/11/30/ArcballCamera/

		arcBallMovementOn = true;

		auto pos = GetLocalPositionV();

		Actor* pickedActor = WorldEditor::GetPickedActor();
		if (pickedActor == nullptr) return;

		auto pivot = pickedActor->GetPositionV();

		XMMATRIX xRot = XMMatrixIdentity();
		xRot = XMMatrixRotationX(dy);
		pos = XMVector3Transform(pos - pivot, xRot) + pivot;

		XMMATRIX yRot = XMMatrixIdentity();
		yRot = XMMatrixRotationY(dx);
		pos = XMVector3Transform(pos - pivot, yRot) + pivot;

		SetWorldPosition(pos);
		SetWorldRotation(VMath::LookAtRotation(pivot, pos));
	}
	else if (Input::GetMouseRightDown())
	{
		Pitch(dy);
		RotateY(dx);
	}

	lastMousePos.x = x;
	lastMousePos.y = y;
}

#include "Camera.h"
#include <math.h>
#include "Input.h"
#include "WorldEditor.h"
#include "Actors/Actor.h"
#include "Editor/Editor.h"
#include "GameUtils.h"
#include "Actors/Game/Player.h"
#include "VMath.h"
#include "Render/Renderer.h"
#include "World.h"

CameraComponent editorCamera(XMFLOAT3(0.f, 2.f, -5.f), true);
CameraComponent* activeCamera;

CameraComponent::CameraComponent(XMFLOAT3 startPos, bool isEditorCamera)
{
	focusPoint = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	transform.position = startPos;
	UpdateTransform();

	editorCamera = isEditorCamera;
}

XMMATRIX CameraComponent::GetViewMatrix()
{
	XMVECTOR forward = GetForwardVectorV();
	XMVECTOR position = GetPositionV();

	XMMATRIX view = XMMatrixIdentity();

	//Player camera logic
	if (!editorCamera)
	{
		if (targetActor)
		{
			focusPoint = targetActor->GetPositionVector();
		}
		view = XMMatrixLookAtLH(transform.world.r[3], focusPoint, VMath::XMVectorUp());
	}
	else
	{
		XMVECTOR focus = position + forward;

		if (arcBallMovementOn)
		{
			focus = worldEditor.pickedActor->GetPositionVector();
		}

		view = XMMatrixLookAtLH(position, focus, VMath::XMVectorUp());
	}

	return view;
}

XMMATRIX CameraComponent::GetProjectionMatrix()
{
	//GetApsectRatio() can change with ResizeBuffer() calls, that's why this getter needs to exist.
	XMMATRIX proj = XMMatrixPerspectiveFovLH(FOV, renderer.GetAspectRatio(), nearZ, farZ);
	return proj;
}

void CameraComponent::Pitch(float angle)
{
	XMMATRIX r = XMMatrixRotationAxis(GetRightVectorV(), angle);
	transform.world *= r;
}

void CameraComponent::RotateY(float angle)
{
	XMMATRIX r = XMMatrixRotationY(angle);
	transform.world *= r;
}

void CameraComponent::MouseMove(int x, int y)
{
	static XMINT2 lastMousePos;

	const float dx = XMConvertToRadians(0.25f * (float)(x - lastMousePos.x));
	const float dy = XMConvertToRadians(0.25f * (float)(y - lastMousePos.y));

	arcBallMovementOn = false;

	if (Input::GetAsyncKey(Keys::MouseRight) && Input::GetAsyncKey(Keys::Alt))
	{
		//Arcball camera movement
		//REF:https://asliceofrendering.com/camera/2019/11/30/ArcballCamera/
		//TODO: sort of works, but keeps reseting the camera's lookat direction when you release the arcball keys
		//because the rotation isn't updated below, only the position.

		arcBallMovementOn = true;

		auto pos = GetPositionV();
		auto pivot = worldEditor.pickedActor->GetPositionVector();
		focusPoint = pivot;

		XMMATRIX xRot = XMMatrixIdentity();
		xRot = XMMatrixRotationX(dy);
		pos = XMVector3Transform(pos - pivot, xRot) + pivot;

		XMMATRIX yRot = XMMatrixIdentity();
		yRot = XMMatrixRotationY(dx);
		pos = XMVector3Transform(pos - pivot, yRot) + pivot;

		SetPosition(pos);
	}
	else if (Input::GetAsyncKey(Keys::MouseRight))
	{
		Pitch(dy);
		RotateY(dx);
	}

	lastMousePos.x = x;
	lastMousePos.y = y;
}

void CameraComponent::Move(float d, XMVECTOR axis)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR position = GetPositionV();
	position = XMVectorMultiplyAdd(s, axis, position);
	SetPosition(position);
}

void CameraComponent::ZoomTo(Actor* actor)
{
	XMVECTOR forward = GetForwardVectorV();

	//Trace the camera down the line its pointing towards the actor
	XMVECTOR actorPos = actor->GetPositionVector();
	XMVECTOR zoomPos = actorPos - (forward * 5.f);

	SetPosition(zoomPos);
}

void CameraComponent::Tick(float deltaTime)
{
	XMVECTOR right = GetRightVectorV();
	XMVECTOR up = GetUpVectorV();
	XMVECTOR forward = GetForwardVectorV();

	if (editorCamera)
	{
		MouseMove(editor->viewportMouseX, editor->viewportMouseY);

		//WASD MOVEMENT
		//if (!gConsole.bConsoleActive)
		{
			if (Input::GetAsyncKey(Keys::MouseRight))
			{
				const float moveSpeed = 7.5f * deltaTime;

				if (Input::GetAsyncKey(Keys::W))
				{
					Move(moveSpeed, forward);
				}
				if (Input::GetAsyncKey(Keys::S))
				{
					Move(-moveSpeed, forward);
				}
				if (Input::GetAsyncKey(Keys::D))
				{
					Move(moveSpeed, right);
				}
				if (Input::GetAsyncKey(Keys::A))
				{
					Move(-moveSpeed, right);
				}
				if (Input::GetAsyncKey(Keys::Q))
				{
					Move(-moveSpeed, up);
				}
				if (Input::GetAsyncKey(Keys::E))
				{
					Move(moveSpeed, up);
				}
			}

			//Zoom onto selected actor
			if (Input::GetKeyUp(Keys::F))
			{
				if (worldEditor.pickedActor)
				{
					ZoomTo(worldEditor.pickedActor);
				}
			}

			//MOUSE WHEEL ZOOM
			const float zoomSpeed = 55.f * deltaTime;

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

void CameraComponent::Create()
{
}

Properties CameraComponent::GetProps()
{
	Properties props("Camera Component");
	props.Add("FOV", &FOV);
	props.Add("Near Z", &nearZ);
	props.Add("Far Z", &farZ);
	return props;
}

void CameraComponent::FrustumCull()
{
	for (auto actor : world.GetAllActorsInWorld())
	{
		XMMATRIX cameraView = GetViewMatrix();
		XMVECTOR cameraViewDet = XMMatrixDeterminant(cameraView);
		XMMATRIX invView = XMMatrixInverse(&cameraViewDet, cameraView);

		XMMATRIX actorWorld = actor->GetWorldMatrix();
		XMVECTOR actorWorldDet = XMMatrixDeterminant(actorWorld);
		XMMATRIX invWorld = XMMatrixInverse(&actorWorldDet, actorWorld);

		XMMATRIX viewToLocal = XMMatrixMultiply(invView, invWorld);

		XMMATRIX proj = GetProjectionMatrix();
		BoundingFrustum frustum, localSpaceFrustum;
		BoundingFrustum::CreateFromMatrix(frustum, proj);
		frustum.Transform(localSpaceFrustum, viewToLocal);

		if (localSpaceFrustum.Contains(actor->rootComponent->boundingBox) == DirectX::DISJOINT)
		{
			actor->active = false;
		}
		else
		{
			actor->active = true;
		}
	}
}

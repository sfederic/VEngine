#include "Camera.h"
#include "Input.h"
#include "WorldEditor.h"
#include "Actors/Actor.h"
#include "Editor/Editor.h"
#include "Math.h"
#include "GameUtils.h"
#include "Actors/Player.h"
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
	XMVECTOR right = transform.world.r[0];
	XMVECTOR up = transform.world.r[1];
	XMVECTOR forward = transform.world.r[2];
	XMVECTOR position = transform.world.r[3];

	forward = XMVector3Normalize(forward);
	up = XMVector3Normalize(XMVector3Cross(forward, right));
	right = XMVector3Cross(up, forward);

	float x = XMVectorGetX(XMVector3Dot(position, right));
	float y = XMVectorGetX(XMVector3Dot(position, up));
	float z = XMVectorGetX(XMVector3Dot(position, forward));

	XMMATRIX view;

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

	//TODO: this camera logic is shit and is just for testing
	//Player camera logic
	if (!editorCamera)
	{
		focusPoint = GameUtils::GetPlayer()->GetPositionVector();
		view = XMMatrixLookAtLH(transform.world.r[3], focusPoint, VMath::XMVectorUp());
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
	XMVECTOR& right = transform.world.r[0];
	XMVECTOR& up = transform.world.r[1];
	XMVECTOR& forward = transform.world.r[2];

	XMMATRIX r = XMMatrixRotationAxis(right, angle);
	up = XMVector3TransformNormal(up, r);
	forward = XMVector3TransformNormal(forward, r);
}

void CameraComponent::RotateY(float angle)
{
	XMVECTOR& right = transform.world.r[0];
	XMVECTOR& up = transform.world.r[1];
	XMVECTOR& forward = transform.world.r[2];

	XMMATRIX r = XMMatrixRotationY(angle);
	up = XMVector3TransformNormal(up, r);
	right = XMVector3TransformNormal(right, r);
	forward = XMVector3TransformNormal(forward, r);
}

void CameraComponent::MouseMove(int x, int y)
{
	static XMINT2 lastMousePos;

	if (Input::GetAsyncKey(Keys::MouseRight))
	{
		float dx = XMConvertToRadians(0.25f * (float)(x - lastMousePos.x));
		float dy = XMConvertToRadians(0.25f * (float)(y - lastMousePos.y));

		Pitch(dy);
		RotateY(dx);
	}

	lastMousePos.x = x;
	lastMousePos.y = y;
}

void CameraComponent::Move(float d, XMVECTOR axis)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR& position = transform.world.r[3];
	position = XMVectorMultiplyAdd(s, axis, position);
}

void CameraComponent::ZoomTo(Actor* actor)
{
	XMVECTOR& position = transform.world.r[3];
	XMVECTOR& forward = transform.world.r[2];

	//Trace the camera down the line its pointing towards the actor
	XMFLOAT3 actorPos = actor->GetPosition();
	XMVECTOR actorPosVec = XMLoadFloat3(&actorPos);
	XMVECTOR zoomPos = actorPosVec - (forward * 5.f);
	position = zoomPos;
}

void CameraComponent::Tick(double deltaTime)
{
	XMVECTOR right = transform.world.r[0];
	XMVECTOR up = transform.world.r[1];
	XMVECTOR forward = transform.world.r[2];

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

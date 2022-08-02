#include "vpch.h"
#include <random>
#include "Camera.h"
#include "Input.h"
#include "WorldEditor.h"
#include "Actors/Actor.h"
#include "Editor/Editor.h"
#include "VMath.h"
#include "Render/Renderer.h"
#include "World.h"
#include "Core.h"
#include "Editor/DebugMenu.h"

CameraComponent editorCamera(XMFLOAT3(0.f, 1.f, -3.f), true);
CameraComponent* activeCamera;

CameraComponent::CameraComponent(XMFLOAT3 startPos, bool isEditorCamera_)
{
	transform.position = startPos;
	UpdateTransform();

	isEditorCamera = isEditorCamera_;
}

XMMATRIX CameraComponent::GetViewMatrix()
{
	XMVECTOR forward = GetForwardVectorV();
	XMVECTOR position = GetPositionV();

	XMMATRIX view = XMMatrixIdentity();

	//Player camera logic
	if (!isEditorCamera)
	{
		if (targetActor)
		{
			focusPoint = targetActor->GetPositionVector();
		}

		//Camera rotation shaking
		//XMVECTOR up = VMath::XMVectorUp() + Shake();
		view = XMMatrixLookAtLH(transform.world.r[3], focusPoint, VMath::XMVectorUp());

		//Camera translation shaking
		XMVECTOR shakeVector = Shake();
		view.r[3] += shakeVector;
	}
	else
	{
		XMVECTOR focus = position + forward;

		if (arcBallMovementOn && WorldEditor::GetPickedActor())
		{
			focus = WorldEditor::GetPickedActor()->GetPositionVector();
		}

		view = XMMatrixLookAtLH(position, focus, VMath::XMVectorUp());
	}

	return view;
}

XMMATRIX CameraComponent::GetProjectionMatrix()
{
	//GetApsectRatio() can change with ResizeBuffer() calls, that's why this getter needs to exist.
	const float FOVRadian = XMConvertToRadians(FOV);
	XMMATRIX proj = XMMatrixPerspectiveFovLH(FOVRadian, Renderer::GetAspectRatio(), nearZ, farZ);
	return proj;
}

void CameraComponent::Pitch(float angle)
{
	XMMATRIX r = XMMatrixRotationAxis(GetRightVectorV(), angle);
	XMVECTOR q = XMQuaternionMultiply(GetRotationV(), XMQuaternionRotationMatrix(r));
	SetRotation(q);
}

void CameraComponent::RotateY(float angle)
{
	XMMATRIX r = XMMatrixRotationY(angle);
	XMVECTOR q = XMQuaternionMultiply(GetRotationV(), XMQuaternionRotationMatrix(r));
	SetRotation(q);
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

//Only works with translation for now.
//Ref: https://gdcvault.com/play/1023146/Math-for-Game-Programmers-Juicing
//@Todo: there's a lot more to do here to give the camera shake a better falloff. Look at ref for ideas.
XMVECTOR CameraComponent::Shake()
{
	if (shakeLevel <= 0.f) return XMVectorZero();
	shakeLevel -= Core::GetDeltaTime();

	static std::random_device randomDevice;
	static std::mt19937 randomGenerator(randomDevice());
	static std::uniform_real_distribution<float> uidDist(-1.f, 1.f);

	const float maxShake = 0.1f;
	float xOffset = maxShake * shakeLevel * uidDist(randomGenerator);
	float yOffset = maxShake * shakeLevel * uidDist(randomGenerator);
	float zOffset = maxShake * shakeLevel * uidDist(randomGenerator);

	XMVECTOR result = XMVectorSet(xOffset, yOffset, zOffset, 0.f); //Make sure the w here is 0
	return result;
}

void CameraComponent::Tick(float deltaTime)
{
	XMVECTOR right = GetRightVectorV();
	XMVECTOR up = GetUpVectorV();
	XMVECTOR forward = GetForwardVectorV();

	if (isEditorCamera)
	{
		MouseMove(editor->viewportMouseX, editor->viewportMouseY);

		//WASD MOVEMENT
		//if (!gConsole.bConsoleActive)
		{
			if (Input::GetAsyncKey(Keys::MouseRight))
			{
				const float moveSpeed = cameraMoveSpeed * deltaTime;

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
					Move(-moveSpeed, VMath::XMVectorUp());
				}
				if (Input::GetAsyncKey(Keys::E))
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
	for (auto actor : World::GetAllActorsInWorld())
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
			actor->SetActive(false);
		}
		else
		{
			actor->SetActive(true);
		}
	}
}

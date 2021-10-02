#include "Player.h"
#include "Components/MeshComponent.h"
#include "Camera.h"
#include "Input.h"
#include "VMath.h"
#include "Physics/Raycast.h"

#include "Editor/Editor.h"
#include "Actors/NPC.h"
#include "Components/DialogueComponent.h"

DialogueComponent* dialogueComponent;

Player::Player()
{
	mesh = MeshComponent::system.Add(this, MeshComponent("cube.fbx", "wall.png"));
	rootComponent = mesh;

	camera = CameraComponent::system.Add(this, CameraComponent(XMFLOAT3(3.f, 3.f, -7.f), false));
	rootComponent->AddChild(camera);
}

void Player::Start()
{
	nextPos = GetPositionVector();

	nextRot = GetRotationVector();
}

void Player::Tick(double deltaTime)
{
	const float moveSpeed = 7.5f;
	SetPosition(VMath::VectorConstantLerp(GetPositionVector(), nextPos, deltaTime, moveSpeed));


	if (Input::GetKeyUp(Keys::Down))
	{
		if (inConversation)
		{
			if (!dialogueComponent->NextLine())
			{
				inConversation = false;
			}
			else
			{ 
				dialogueComponent->ShowTextAtActor();
			}
		}
		else
		{
			Ray ray;
			ray.actorsToIgnore.push_back(this);
			if (Raycast(ray, GetPositionVector(), GetForwardVectorV(), 1.5f))
			{
				NPC* npc = (NPC*)ray.hitActor;
				if (npc)
				{
					dialogueComponent = npc->dialogue;
					inConversation = true;

					dialogueComponent->ShowTextAtActor();
				}
			}
		}
	}


	if (XMVector4Equal(GetPositionVector(), nextPos) && XMQuaternionEqual(GetRotationVector(), nextRot))
	{
		XMVECTOR previousPos = nextPos;

		if (Input::GetAsyncKey(Keys::W))
		{
			nextPos = GetPositionVector() + GetForwardVectorV();
		}		
		if (Input::GetAsyncKey(Keys::S))
		{
			nextPos = GetPositionVector() + -GetForwardVectorV();
		}
		if (Input::GetAsyncKey(Keys::A))
		{
			nextPos = GetPositionVector() + -GetRightVectorV();
		}
		if (Input::GetAsyncKey(Keys::D))
		{
			nextPos =  GetPositionVector() + GetRightVectorV();
		}

		if (!XMVector4Equal(previousPos, nextPos))
		{
			Ray ray;
			ray.actorsToIgnore.push_back(this);
			XMVECTOR direction = XMVector3Normalize(nextPos - previousPos);
			if (Raycast(ray, GetPositionVector(), direction, 1.f))
			{
				nextPos = previousPos;
			}
		}
	}

	const float rotSpeed = 5.f;
	SetRotation(VMath::QuatConstantLerp(GetRotationVector(), nextRot, deltaTime, rotSpeed));

	if (XMQuaternionEqual(GetRotationVector(), nextRot) && XMVector4Equal(GetPositionVector(), nextPos))
	{
		if (Input::GetKeyUp(Keys::Right))
		{
			const float angle = XMConvertToRadians(90.f);
			nextRot = XMQuaternionMultiply(nextRot, DirectX::XMQuaternionRotationAxis(VMath::XMVectorUp(), angle));
		}		
		if (Input::GetKeyUp(Keys::Left))
		{
			const float angle = XMConvertToRadians(-90.f);
			nextRot = XMQuaternionMultiply(nextRot, DirectX::XMQuaternionRotationAxis(VMath::XMVectorUp(), angle));
		}
	}
}

Properties Player::GetProps()
{
    return Actor::GetProps();
}

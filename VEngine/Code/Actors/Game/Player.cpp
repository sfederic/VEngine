#include "Player.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera.h"
#include "Input.h"
#include "VMath.h"
#include "Physics/Raycast.h"
#include "GameUtils.h"
#include "Editor/Editor.h"
#include "Actors/Game/NPC.h"
#include "Actors/Game/Pickup.h"
#include "Components/DialogueComponent.h"
#include "Actors/Game/BattleGrid.h"

DialogueComponent* dialogueComponent;

Player::Player()
{
	mesh = MeshComponent::system.Add(this, MeshComponent("cube.fbx", "wall.png"));
	rootComponent = mesh;

	camera = CameraComponent::system.Add(this, CameraComponent(XMFLOAT3(3.f, 3.f, -7.f), false));
	rootComponent->AddChild(camera);

	widget = WidgetComponent::system.Add(this);
}

void Player::Start()
{
	nextPos = GetPositionVector();
	nextRot = GetRotationVector();
}

void Player::Tick(float deltaTime)
{
	const float moveSpeed = 5.5f;
	SetPosition(VMath::VectorConstantLerp(GetPositionVector(), nextPos, deltaTime, moveSpeed));

	//toggle battlegrid visibility
	if (Input::GetKeyUp(Keys::B))
	{
		auto battleGrid = GameUtils::GetBattleGrid();
		battleGrid->active = !battleGrid->active;
	}

	if (Input::GetKeyUp(Keys::Down))
	{
		GameUtils::PlayAudio("jingle.wav");

		Ray ray(this);
		if (Raycast(ray, GetPositionVector(), GetForwardVectorV(), 1.5f))
		{
			//PICKUP CHECK
			{
				auto pickup = dynamic_cast<Pickup*>(ray.hitActor);
				if (pickup)
				{
					heldItem = pickup;
					pickup->AddToPlayerInventory();
					return;
				}
			}

			//DIALOGUE CHECK
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
				NPC* npc = dynamic_cast<NPC*>(ray.hitActor);
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
			Ray ray(this);
			XMVECTOR direction = XMVector3Normalize(nextPos - previousPos);
			if (Raycast(ray, GetPositionVector(), direction, 1.f))
			{
				nextPos = previousPos;
			}
		}
	}

	const float rotSpeed = 5.0f;
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

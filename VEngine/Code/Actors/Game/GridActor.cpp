#include "vpch.h"
#include "GridActor.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Game/MemoryComponent.h"
#include "Components/Game/DialogueComponent.h"
#include "UI/Game/HealthWidget.h"
#include "Grid.h"
#include "Core/VMath.h"
#include "Physics/Raycast.h"
#include "Gameplay/GameUtils.h"

GridActor::GridActor()
{
	mesh = MeshComponent::system.Add("Mesh", this);
	rootComponent = mesh;

	memoryComponent = MemoryComponent::system.Add("Memory", this);
	dialogueComponent = DialogueComponent::system.Add("Dialogue", this);
}

void GridActor::Start()
{
	SetGridPosition();

	if (!IsActive())
	{
		auto node = GetCurrentNode();
		if (node)
		{
			node->Show();
		}
	}
}

void GridActor::Tick(float deltaTime)
{
	dialogueComponent->SetPosition(GetHomogeneousPositionV());

	if (isInPushback)
	{
		XMVECTOR position = GetPositionV();
		if (!VMath::VecEqual(position, nextPushbackPosition, 0.25f))
		{
			SetPosition(XMVectorLerp(GetPositionV(), nextPushbackPosition, deltaTime * 6.5));

			XMVECTOR rot = GetRotationV();
			SetRotation(XMQuaternionMultiply(rot, XMQuaternionRotationAxis(-GetForwardVectorV(), 5.f * deltaTime)));
		}
		else
		{
			if (hitActorOnPushback)
			{
				hitActorOnPushback->InflictDamage(1);
			}

			isInPushback = false;
			GameUtils::SpawnSpriteSheet("Sprites/explosion.png", GetPositionV(), false, 4, 4);
			GameUtils::PlayAudioOneShot("Audio/armor_light.wav");
			Destroy();
		}
	}
}

Properties GridActor::GetProps()
{
	auto props = Actor::GetProps();
	props.title = "GridActor";
	props.Add("Destruct", &isDestructible);
	props.Add("Health", &health);
	props.Add("Interact", &isInteractable);
	props.Add("Inspect", &isInspectable);
	props.Add("Interact Text", &interactText);
	props.Add("InteractKnownText", &interactKnownText);
	props.Add("DisableGridInteract", &disableGridInteract);
	props.Add("Obstacle", &isGridObstacle);
	return props;
}

void GridActor::InflictDamage(int damage)
{
	Attacked();

	if (!isDestructible) return;

	health -= damage;
	if (healthWidget)
	{
		healthWidget->healthPoints = health;
	}

	if (health <= 0)
	{
		GetCurrentNode()->Show();
		HitResult hit(this);
		GetCurrentNode()->RecalcNodeHeight(hit);
		Destroy();
	}
}

void GridActor::SetGridPosition()
{
	xIndex = std::round(GetPosition().x);
	yIndex = std::round(GetPosition().z);
}

GridNode* GridActor::GetCurrentNode()
{
	if (disableGridInteract)
	{
		return nullptr;
	}

	auto grid = Grid::system.GetFirstActor();
	auto node = grid->GetNode(xIndex, yIndex);
	return node;
}

bool GridActor::Pushback(XMVECTOR direction)
{
	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), direction, 50.f))
	{
		nextPushbackPosition = XMLoadFloat3(&hit.hitPos);
		isInPushback = true;
		hitActorOnPushback = dynamic_cast<GridActor*>(hit.hitActor);

		HitResult nodeRecalcHit(this);
		GetCurrentNode()->RecalcNodeHeight(nodeRecalcHit);

		return true;
	}

	return false;
}

ForwardFace GridActor::GetCurrentForwardFace()
{
	const XMFLOAT3 forward = GetForwardVector();

	const int forwardIndex = static_cast<int>(std::lroundf(forward.z));
	const int rightIndex = static_cast<int>(std::lroundf(forward.x));

	if (forwardIndex > 0) return ForwardFace::positiveZ;
	else if (forwardIndex < 0) return ForwardFace::negativeZ;
	else if (rightIndex > 0) return ForwardFace::positiveX;
	else if (rightIndex < 0) return ForwardFace::negativeX;

	throw new std::exception("something went wrong with forward vector");
}

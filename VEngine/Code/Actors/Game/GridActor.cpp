#include "vpch.h"
#include "GridActor.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Game/MemoryComponent.h"
#include "Components/Game/DialogueComponent.h"
#include "UI/Game/HealthWidget.h"
#include "Grid.h"
#include "VMath.h"
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
			GameUtils::SpawnSpriteSheet("Sprites/explosion.png", GetPosition(), false, 4, 4);
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
	props.AddProp(interactKnownText);
	props.AddProp(disableGridInteract);
	props.Add("Obstacle", &isGridObstacle);
	return props;
}

void GridActor::InflictDamage(int damage)
{
	if (!isDestructible) return;

	health -= damage;
	if (healthWidget)
	{
		healthWidget->healthPoints = health;
	}

	if (health <= 0)
	{
		GetCurrentNode()->Show();
		Ray ray(this);
		GetCurrentNode()->RecalcNodeHeight(ray);
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

void GridActor::SetAnimation(std::string animationName)
{
	auto skeleton = mesh->GetSkeleton();
	if (!skeleton->animations.empty())
	{
		skeleton->currentAnimation = animationName;
	}
}

bool GridActor::Pushback(XMVECTOR direction)
{
	Ray ray(this);
	if (Raycast(ray, GetPositionV(), direction, 50.f))
	{
		nextPushbackPosition = XMLoadFloat3(&ray.hitPos);
		isInPushback = true;
		hitActorOnPushback = dynamic_cast<GridActor*>(ray.hitActor);

		Ray nodeRecalcRay(this);
		GetCurrentNode()->RecalcNodeHeight(nodeRecalcRay);

		return true;
	}

	return false;
}

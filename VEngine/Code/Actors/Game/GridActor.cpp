#include "GridActor.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/MemoryComponent.h"
#include "Components/TimeComponent.h"
#include "Components/DialogueComponent.h"
#include "Components/QuestComponent.h"
#include "UI/HealthWidget.h"
#include "Gameplay/GameUtils.h"
#include "Grid.h"

GridActor::GridActor()
{
	mesh = MeshComponent::system.Add(this);
	rootComponent = mesh;

	timeComponent = TimeComponent::system.Add(this);
	memoryComponent = MemoryComponent::system.Add(this);
	dialogueComponent = DialogueComponent::system.Add(this);
	questComponent = QuestComponent::system.Add(this);
}

void GridActor::Start()
{
	SetGridPosition();

	if (!EnableBasedOnTime() && isGridObstacle)
	{
		//Set starting node as inactive.
		auto node = GetCurrentNode();
		node->Hide();
	}
}

void GridActor::Tick(float deltaTime)
{
	dialogueComponent->SetPosition(GetHomogeneousPositionVector());
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
		//Deactive quest linked to GridActor on death/destruction
		questComponent->DeactivateQuest();
		
		GetCurrentNode()->Show();
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
	auto grid = GameUtils::GetGrid();
	auto node = grid->GetNode(xIndex, yIndex);
	return node;
}

bool GridActor::EnableBasedOnTime()
{
	if (!timeComponent->CheckIfActiveAtCurrentTime())
	{
		SetActive(false);
		return true;
	}

	return false;
}

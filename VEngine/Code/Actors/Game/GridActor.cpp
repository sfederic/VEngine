#include "vpch.h"
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

	//Disable actor based on quest
	if (!questComponent->CheckIfQuestActive())
	{
		SetActive(false);
	}

	if (!EnableBasedOnTime() && isGridObstacle)
	{
		auto node = GetCurrentNode();
		if(node)
		{
			node->Hide();
		}
	}

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
	if (disableGridInteract)
	{
		return nullptr;
	}

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

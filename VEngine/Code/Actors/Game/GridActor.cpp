#include "GridActor.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/IntuitionComponent.h"
#include "Components/TimeOfDayComponent.h"
#include "UI/HealthWidget.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/BattleGrid.h"

GridActor::GridActor()
{
	mesh = MeshComponent::system.Add(this);
	rootComponent = mesh;

	timeOfDayComponent = TimeOfDayComponent::system.Add(this);
	intuition = IntuitionComponent::system.Add(this);
}

void GridActor::Start()
{
	if (timeOfDayComponent->CheckIfActiveAtCurrentTime())
	{
		SetActive(false);
		return;
	}

	SetGridPosition();

	healthWidget = CreateWidget<HealthWidget>();
	healthWidget->healthPoints = health;
}

void GridActor::Tick(float deltaTime)
{
	healthWidget->pos = GetHomogeneousPositionVector();
}

Properties GridActor::GetProps()
{
	auto props = Actor::GetProps();
	props.title = "GridActor";
	props.Add("Destruct", &isDestructible);
	props.Add("Health", &health);
	props.Add("Interact", &isInteractable);
	props.Add("Interact Text", &interactText);
	props.Add("Obstacle", &isGridObstacle);
	return props;
}

void GridActor::InflictDamage(int damage)
{
	if (!isDestructible) return;

	health -= damage;
	healthWidget->healthPoints = health;

	if (health <= 0)
	{
		Destroy();
	}
}

void GridActor::SetGridPosition()
{
	xIndex = std::round(GetPosition().x);
	yIndex = std::round(GetPosition().z);
}

GridNode* GridActor::GeCurrentNode()
{
	auto battleGrid = GameUtils::GetBattleGrid();
	auto node = battleGrid->GetNode(xIndex, yIndex);
}

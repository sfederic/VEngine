#include "GridActor.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/IntuitionComponent.h"
#include "UI/HealthWidget.h"

GridActor::GridActor()
{
	mesh = MeshComponent::system.Add(this);
	rootComponent = mesh;

	intuition = IntuitionComponent::system.Add(this);
}

void GridActor::Start()
{
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

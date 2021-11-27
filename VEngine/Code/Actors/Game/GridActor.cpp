#include "GridActor.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/HealthWidget.h"
#include "UI/UISystem.h"

GridActor::GridActor()
{
	mesh = MeshComponent::system.Add(this);
	rootComponent = mesh;

	//widget = WidgetComponent::system.Add(this);
}

void GridActor::Start()
{
	healthWidget = uiSystem.CreateWidget<HealthWidget>();
	healthWidget->healthPoints = &health;
	healthWidget->AddToViewport();
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
	return props;
}

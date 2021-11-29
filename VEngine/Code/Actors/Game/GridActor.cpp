#include "GridActor.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/HealthWidget.h"
#include "UI/UISystem.h"
#include "Game/IntuitionSystem.h"

GridActor::GridActor()
{
	mesh = MeshComponent::system.Add(this);
	rootComponent = mesh;
}

void GridActor::Start()
{
	SetIntuition();

	healthWidget = uiSystem.CreateWidget<HealthWidget>();
	healthWidget->healthPoints = &health;
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
	props.Add("Intuition", &intuitionName);
	return props;
}

void GridActor::SetIntuition()
{
	//Set intuition
	if (!intuitionName.empty())
	{
		auto foundIntuition = intuitionSystem.FindIntution(intuitionName);
		intuition = foundIntuition;
	}
}

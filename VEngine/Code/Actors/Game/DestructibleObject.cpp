#include "vpch.h"
#include "DestructibleObject.h"
#include "UI/Game/HealthWidget.h"
#include "UI/UISystem.h"

void DestructibleObject::Start()
{
	healthWidget = UISystem::CreateWidget<HealthWidget>();
	healthWidget->AddToViewport();
}

void DestructibleObject::Tick(float deltaTime)
{
	healthWidget->SetHealth(health);
	healthWidget->worldPosition = GetHomogeneousPositionV();
}

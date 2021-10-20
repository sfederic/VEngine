#include "SavePoint.h"
#include "Components/BoxTriggerComponent.h"

SavePoint::SavePoint()
{
	trigger = BoxTriggerComponent::system.Add(this);
	rootComponent = trigger;
}

void SavePoint::Tick(float deltaTime)
{
}

Properties SavePoint::GetProps()
{
	return Actor::GetProps();
}

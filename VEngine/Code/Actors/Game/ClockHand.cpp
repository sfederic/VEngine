#include "vpch.h"
#include "ClockHand.h"
#include "Components/SpatialComponent.h"

ClockHand::ClockHand()
{
	SetEmptyRootComponent();
	rootComponent->AddChild((SpatialComponent*)mesh);
}

Properties ClockHand::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

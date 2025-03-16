
#include "ClockHand.h"
#include "Components/SpatialComponent.h"

ClockHand::ClockHand()
{
	SetEmptyRootComponent();
	AddChildToRoot((SpatialComponent*)_mesh);
}

Properties ClockHand::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

#include "vpch.h"
#include "PolyboardActor.h"
#include "Particle/Polyboard.h"

PolyboardActor::PolyboardActor()
{
	polyboard = CreateComponent<Polyboard>("PolyBoard");
	SetRootComponent(polyboard);
}

Properties PolyboardActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

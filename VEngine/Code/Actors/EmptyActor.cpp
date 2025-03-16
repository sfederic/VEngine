
#include "EmptyActor.h"

EmptyActor::EmptyActor()
{
	SetEmptyRootComponent();
}

Properties EmptyActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

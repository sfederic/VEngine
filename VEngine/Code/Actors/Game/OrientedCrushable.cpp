
#include "OrientedCrushable.h"
import Core.VMath;

Properties OrientedCrushable::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void OrientedCrushable::Crushed()
{
	if (VMath::VecEqual(GetForwardVectorV(), VMath::GlobalUpVector()))
	{
		__super::Crushed();
	}
}

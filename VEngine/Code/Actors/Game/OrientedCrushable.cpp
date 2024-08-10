#include "vpch.h"
#include "OrientedCrushable.h"
#include "Core/VMath.h"

void OrientedCrushable::Crushed()
{
	if (VMath::VecEqual(GetForwardVectorV(), VMath::GlobalUpVector()))
	{
		__super::Crushed();
	}
}

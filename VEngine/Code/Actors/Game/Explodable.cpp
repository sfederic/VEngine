#include "vpch.h"
#include "Explodable.h"

void Explodable::HitByExplosive()
{
	Destroy();
}

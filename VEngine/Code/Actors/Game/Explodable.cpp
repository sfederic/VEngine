
#include "Explodable.h"

Properties Explodable::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void Explodable::HitByExplosive()
{
	Remove();
}

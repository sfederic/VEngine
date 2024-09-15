#include "vpch.h"
#include "BombRock.h"

Properties BombRock::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void BombRock::OnLinkDeactivate()
{
	__super::OnLinkDeactivate();

	Burn();
}

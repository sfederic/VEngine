#include "vpch.h"
#include "BombRock.h"

void BombRock::OnLinkDeactivate()
{
	__super::OnLinkDeactivate();

	Burn();
}

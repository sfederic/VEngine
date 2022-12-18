#include "vpch.h"
#include "Enemy.h"

Enemy::Enemy()
{
	SetEmptyRootComponent();
}

Properties Enemy::GetProps()
{
	auto props = __super::GetProps();
	props.title = "Enemy";
	return props;
}

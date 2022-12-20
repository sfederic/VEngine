#include "vpch.h"
#include "Enemy.h"
#include "Gameplay/GameplayTags.h"

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

bool Enemy::CheckIfAllTaggedMeshesAreDestroyed()
{
	for (auto& componentPair : componentMap)
	{
		if (componentPair.second->HasTag(GameplayTags::EnemyMeshPiece))
		{
			return false;
		}
	}

	return true;
}

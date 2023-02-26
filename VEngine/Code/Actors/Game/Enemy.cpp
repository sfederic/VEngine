#include "vpch.h"
#include "Enemy.h"
#include "Gameplay/GameplayTags.h"
#include "Gameplay/GameUtils.h"
#include "Player.h"

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

void Enemy::OnDestroyed()
{
	Destroy();

	auto enemies = World::GetAllActorsOfTypeInWorld<Enemy>();
	if (enemies.empty())
	{
		GameUtils::TriggerLevelComplete();
	}
}

void Enemy::InflictDamageToActor(Actor* actor)
{
	auto player = dynamic_cast<Player*>(actor);
	if (player)
	{
		player->InflictDamage(1.f);
	}
}

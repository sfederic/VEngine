#include "vpch.h"
#include "Enemy.h"
#include "Gameplay/GameplayTags.h"
#include "Gameplay/GameUtils.h"
#include "Player.h"

Enemy::Enemy()
{
	SetEmptyRootComponent();
}

void Enemy::Create()
{
	tags.emplace(GameplayTags::TargetableActor);
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

void Enemy::InflictDamage(int damage)
{
	health -= damage;
	if (HasHealthDepleted())
	{
		OnDestroyed();
	}
}

void Enemy::OnDestroyed()
{
	auto enemies = World::GetAllActorsOfTypeInWorld<Enemy>();
	if (enemies.empty())
	{
		GameUtils::TriggerLevelComplete();
	}

	Destroy();
}

void Enemy::InflictDamageToActor(Actor* actor)
{
	auto player = dynamic_cast<Player*>(actor);
	if (player)
	{
		player->InflictDamage(1.f);
	}
}

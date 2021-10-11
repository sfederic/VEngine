#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct MeshComponent;

struct BattleNode : Actor
{
	ACTOR_SYSTEM(BattleNode)

	MeshComponent* nodeMesh = nullptr;

	BattleNode();
	virtual Properties GetProps() override;
};

#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct InstanceMeshComponent;

struct BattleGrid : Actor
{
	ACTOR_SYSTEM(BattleGrid)

	InstanceMeshComponent* nodeMesh = nullptr;

	int sizeX;
	int sizeY;

	BattleGrid();
	virtual void Start() override;
	virtual Properties GetProps() override;
};

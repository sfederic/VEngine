#pragma once
#include "Actor.h"
#include "ActorSystem.h"
#include "GridNode.h"

struct InstanceMeshComponent;

struct GridRow
{
	std::vector<GridNode> nodes;

	void Add(GridNode node)
	{
		nodes.emplace_back(node);
	}
};

struct BattleGrid : Actor
{
	ACTOR_SYSTEM(BattleGrid)

	InstanceMeshComponent* nodeMesh = nullptr;

	std::vector<GridRow> rows;

	int sizeX = 1;
	int sizeY = 1;

	BattleGrid();
	virtual void Start() override;
	virtual Properties GetProps() override;
};

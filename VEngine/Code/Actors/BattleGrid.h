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

//Actor that holds all the traversable nodes in the level.
//BatteGrid needs to always be at (0, 0, 0) in world because of how rows & nodes are created at index.
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

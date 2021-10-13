#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct GridNode;
struct MeshComponent;

//Units are all moveable actors that can be placed in-world
struct Unit : Actor
{
	ACTOR_SYSTEM(Unit)

	MeshComponent* mesh = nullptr;

	int xIndex = -1;
	int yIndex = -1;

	int movementPoints = 20;

	int movementPathNodeIndex = 0;

	std::vector<GridNode*> movementPathNodes;
	std::vector<GridNode*> pathNodes;

	XMVECTOR nextMovePos;

	Unit();
	virtual void Start() override;
	virtual void Tick(double deltaTime) override;
	virtual Properties GetProps() override;
	void MoveTo(GridNode* destinationNode);
};

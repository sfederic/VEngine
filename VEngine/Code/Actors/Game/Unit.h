#pragma once
#include "GridActor.h"
#include "../ActorSystem.h"

struct GridNode;

//Units are battle ready actors and really only move and fight.
struct Unit : GridActor
{
	ACTOR_SYSTEM(Unit)

	int xIndex = -1;
	int yIndex = -1;

	int movementPoints = 1;

	int movementPathNodeIndex = 0;

	bool isInBattle = false;

	//All the nodes the unit can move to
	std::vector<GridNode*> movementPathNodes;

	//The end path the unit takes after a call to MoveTo()
	std::vector<GridNode*> pathNodes;

	XMVECTOR nextMovePos;

	Unit();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
	void MoveTo(GridNode* destinationNode);
};

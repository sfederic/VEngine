#pragma once
#include "NPC.h"
#include "../ActorSystem.h"

struct GridNode;
struct MeshComponent;

//Units are all moveable actors that can be placed in-world
struct Unit : NPC
{
	ACTOR_SYSTEM(Unit)

	MeshComponent* mesh = nullptr;

	int xIndex = -1;
	int yIndex = -1;

	int movementPoints = 1;

	int movementPathNodeIndex = 0;

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

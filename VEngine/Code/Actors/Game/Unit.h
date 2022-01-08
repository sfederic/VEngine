#pragma once
#include "GridActor.h"

struct GridNode;

//Units are battle ready actors and really only move and fight.
struct Unit : GridActor
{
	ACTOR_SYSTEM(Unit);

	enum class BattleStates
	{
		Fight,
		Flee,
		Wander
	};

	//Text to display on unit's death during battle
	std::wstring deathText;

	float moveSpeed = 1.0f;

	int movementPoints = 1;
	int movementPathNodeIndex = 0;

	int attackPoints = 1;

	bool isInBattle = false;

private:
	bool isUnitTurn = false;
	bool attackWindingUp = false;

public:
	//All the nodes the unit can move to
	std::vector<GridNode*> movementPathNodes;

	//The end path the unit takes after a call to MoveToNode()
	std::vector<GridNode*> pathNodes;

	XMVECTOR nextMovePos;

	Unit();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	virtual void InflictDamage(int damage) override;

	void MoveToNode(GridNode* destinationNode);
	void MoveToNode(int x, int y);

	//Figure out movement path and target during battle on turn start
	void StartTurn();
	void EndTurn();

	bool Attack();
	void WindUpAttack();
};

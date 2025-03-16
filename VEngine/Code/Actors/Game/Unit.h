#pragma once

#include "GridActor.h"
#include <memory>
import Core.VEnum;
#include "Gameplay/BattleEnums.h"

struct GridNode;
class EntranceTrigger;
class Polyboard;
class Player;

//Units are battle ready actors and really only move and fight.
class Unit : public GridActor
{
public:
	ACTOR_SYSTEM(Unit);

	//Battle states
	struct BattleStates
	{
		inline static std::string fight = "fight"; //Move towards player/destination
		inline static std::string evade = "evade"; //Move away from destination
		inline static std::string wander = "wander"; //Move around randomly
	};

	VEnum battleState;

	//Text to display on unit's death during battle
	std::wstring deathText;

	size_t movementPathNodeIndex = 0;

	int movementPoints = 1;

	int numOfAttacks = 1;

	int attackPoints = 1;

	int attackRange = 1; //Attack range should always be >= 1

	//All directions the Unit can be successfully attacked from.
	AttackDirection attackDirections = AttackDirection::All;

	//All the nodes the unit can move to
	std::vector<GridNode*> movementPathNodes;

	//The end path the unit takes after a call to MoveToNode()
	std::vector<GridNode*> pathNodes;

	//This is the actor name a Unit is focusing its 'intent' on. It can be another Unit, or Actor.
	std::string actorToFocusOn;

	Unit();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
	virtual void InflictDamage(int damage) override;
	void MoveToNode(GridNode* destinationNode);
	void MoveToNode(int x, int y);
	void MoveToRandomNodeInMovementRange();
	void StartTurn();
	bool Attack();
	void WindUpAttack();
	void ShowUnitMovementPath();
	void SetUnitLookAt(DirectX::XMVECTOR lookAtPoint);

	//This is for ending Unit's movement that turn.
	void SetMovePathIndexToMax();

private:
	std::vector<GridNode*> GetMovementPathPreviewNodes();
};

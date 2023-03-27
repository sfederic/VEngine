#pragma once

#include "GridActor.h"
#include <memory>
#include "Core/VEnum.h"
#include "Gameplay/BattleEnums.h"

struct GridNode;
struct MemoryComponent;
struct EntranceTrigger;
struct Polyboard;
struct Memory;
class PlayerUnit;

//Units are battle ready actors and really only move and fight.
struct Unit : GridActor
{
	ACTOR_SYSTEM(Unit);

	//Battle states
	struct BattleStates
	{
		inline static std::string fight = "fight"; //Move towards player/destination
		inline static std::string evade = "evade"; //Move away from destination
		inline static std::string wander = "wander"; //Move around randomly
		inline static std::string escape = "escape"; //Esacpe to nearest EntranceTrigger
	};

	VEnum battleState;

	MemoryComponent* memoryOnDeath = nullptr;

	//Meant to show a unit's current focus in battle and in world
	Polyboard* intentBeam = nullptr;

	EntranceTrigger* entranceToEscapeTo = nullptr;

	//Text to display on unit's death during battle
	std::wstring deathText;

	float moveSpeed = 3.0f;

	int movementPoints = 1;
	int movementPathNodeIndex = 0;

	int numOfAttacks = 1;

	int attackPoints = 1;

	int attackRange = 1; //Attack range should always be >= 1

	bool isInBattle = false;

	bool isInTrapNode = false;

	//All directions the Unit can be successfully attacked from.
	AttackDirection attackDirections = AttackDirection::All;

private:
	bool isUnitTurn = false;
	bool attackWindingUp = false;

public:
	//All the nodes the unit can move to
	std::vector<GridNode*> movementPathNodes;

	//The end path the unit takes after a call to MoveToNode()
	std::vector<GridNode*> pathNodes;

	XMVECTOR nextMovePos;

	//This is the actor name a Unit is focusing its 'intent' on. It can be another Unit, or Actor.
	std::string actorToFocusOn;

	Unit();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
	virtual void InflictDamage(int damage) override;
	void MoveToNode(GridNode* destinationNode);
	void MoveToNode(int x, int y);
	void StartTurn();
	void EndTurn();
	bool Attack();
	void WindUpAttack();
	void ShowUnitMovementPath();
	void SetUnitLookAt(XMVECTOR lookAtPoint);

	//This is for ending Unit's movement that turn.
	void SetMovePathIndexToMax();

private:
	std::vector<GridNode*> GetMovementPathPreviewNodes(GridNode* destinationNode);
	PlayerUnit* FindClosestPlayerUnit();
};

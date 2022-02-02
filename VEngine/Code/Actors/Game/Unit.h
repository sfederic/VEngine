#pragma once
#include "GridActor.h"
#include "VEnum.h"

struct GridNode;
struct MemoryComponent;

//Units are battle ready actors and really only move and fight.
struct Unit : GridActor
{
	ACTOR_SYSTEM(Unit);

	//Battle states
	struct BattleStates
	{
		inline static std::string fight = "fight";
		inline static std::string flee = "flee";
		inline static std::string wander = "wander";
	};

	VEnum battleState;

	MemoryComponent* memoryOnDeath = nullptr;

	//Text to display on unit's death during battle
	std::wstring deathText;

	float moveSpeed = 3.0f;

	int movementPoints = 1;
	int movementPathNodeIndex = 0;

	int attackPoints = 1;

	int attackRange = 1; //Attack range should always be >= 1

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

	void ShowUnitMovementPath();

	void SetUnitLookAt(XMVECTOR lookAtPoint);

private:
	std::vector<GridNode*> GetMovementPathPreviewNodes(GridNode* destinationNode);
};

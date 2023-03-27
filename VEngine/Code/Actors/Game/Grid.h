#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"
#include "Gameplay/GridNode.h"

struct InstanceMeshComponent;
class Unit;
class PlayerUnit;

//Actor that holds all the traversable nodes in the level.
//Grid needs to always be at (0, 0, 0) in world because of how rows & nodes are created at index.
struct Grid : public Actor
{
	ACTOR_SYSTEM(Grid);

	struct GridRow
	{
		std::vector<GridNode> columns;

		void Add(GridNode node)
		{
			columns.emplace_back(node);
		}
	};

	InstanceMeshComponent* nodeMesh = nullptr;

	std::vector<GridRow> rows;

	inline static float maxHeightMove = 1.0f;

	int sizeX = 1;
	int sizeY = 1;

	bool startBattleOnLoad = false;

	enum class LerpValue
	{
		LerpIn,
		LerpOut
	};

	LerpValue lerpValue = LerpValue::LerpOut;

	Grid();
	virtual void Awake() override;
	virtual void Start() override;
	virtual void LateStart() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
	GridNode* GetNode(int x, int y);
	GridNode* GetNodeAllowNull(int x, int y);

	std::vector<GridNode*> GetAllNodes();

	//Limit the node gotten between 0 and the size of the grid.
	GridNode* GetNodeLimit(int x, int y);

	void GetNeighbouringNodes(GridNode* centerNode, std::vector<GridNode*>& outNodes);

	//Get neighbouring nodes without consideration for whether they're closed or their world position (only active nodes count).
	void GetNeighbouringNodesForceful(GridNode* centerNode, std::vector<GridNode*>& outNodes);

	//Same as GetNeighbouringNodesForceful() except gets all inactive nodes too.
	std::vector<GridNode*> GetNeighbouringActiveAndInactiveNodesForceful(GridNode* centerNode);

	//Returns Unit currently at GridNode x and y index (only returns first found Unit)
	Unit* GetUnitAtNode(GridNode* node);
	Unit* GetUnitAtNodeIndex(int xIndex, int yIndex);
	std::vector<Unit*> GetAllUnitsFromNodes(std::vector<GridNode*>& nodes);

	//For PlayerUnit fusion battle mechanic.
	std::vector<PlayerUnit*> GetAllPlayerUnitsAtNode(GridNode* node);

	void ResetAllNodes();
	void LerpInNodes(float deltaTime);
	void LerpOutNodes(float deltaTime);
	void DisplayHideAllNodes();
	void DisplayShowAllNodes();
	void DisarmAllTrapNodes();
};

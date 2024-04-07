#pragma once

#include <vector>
#include "../Actor.h"
#include "../ActorSystem.h"
#include "Gameplay/GridNode.h"

struct InstanceMeshComponent;
struct InstanceData;
class Unit;
class GridActor;

//@Todo: if pathfinding needs a more grid-less solution,
//look to https://docs.unrealengine.com/udk/Three/NavigationMeshReference.html

//Actor that holds all the traversable nodes in the level.
//Grid needs to always be at (0, 0, 0) in world because of how rows & nodes are created at index.
class Grid : public Actor
{
public:
	ACTOR_SYSTEM(Grid);

	inline static float maxHeightMove = 1.1f;
	inline static float maxPlayerDropHeight = 1.9f;

private:
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

	int sizeX = 1;
	int sizeY = 1;

	enum class LerpSetting
	{
		LerpIn,
		LerpOut
	};

	LerpSetting lerpSetting = LerpSetting::LerpIn;
	float currentLerpValue = 0.0f;

public:
	Grid();
	void Awake() override;
	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;
	GridNode* GetNode(int x, int y);
	GridNode* GetNodeAllowNull(int x, int y);

	std::vector<GridNode*> GetAllNodes();

	//With 'preserveNodeScaleValues', what happens is that when the grid buffers are recreated,
	//the individual node lerp values are reset because the original vectors are reset. 
	//Use this bool to keep that original lerped value after recreating buffers.
	void RecalcAllNodes(HitResult& hit, bool preserveNodeScaleValues);

	//This function exists to make the grid rules clearer. The intent is when you link to a grid actor,
	//you want its own nodes to be recalced so that grid actors bigger than 1x1 aren't moving to their 
	//own nodes on movement.
	void RecalcNodesToIgnoreLinkedGridActor(GridActor* gridActorToIgnore);

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

	void ResetAllNodes();
	void LerpInNodes(float deltaTime);
	void LerpOutNodes(float deltaTime);
	void DisplayHideAllNodes();
	void DisplayShowAllNodes();

	std::vector<InstanceData>& GetNodeMeshInstanceData();

	auto GetSizeX() const { return sizeX; }
	auto GetSizeY() const { return sizeY; }
	void SetGridSize(int x, int y);

	bool IsLerpingIn() const { return lerpSetting == LerpSetting::LerpIn; }
	bool IsLerpingOut() const { return lerpSetting == LerpSetting::LerpOut; }
	void SetLerpIn() { lerpSetting = LerpSetting::LerpIn; }
	void SetLerpOut() { lerpSetting = LerpSetting::LerpOut; }

private:
	void SetAllNodesToCurrentLerpValue();
};

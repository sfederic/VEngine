#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"
#include "Gameplay/GridNode.h"

struct InstanceMeshComponent;

//Actor that holds all the traversable nodes in the level.
//Grid needs to always be at (0, 0, 0) in world because of how rows & nodes are created at index.
struct Grid : Actor
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

	enum class LerpValue
	{
		LerpIn,
		LerpOut
	};

	LerpValue lerpValue = LerpValue::LerpOut;

	Grid();
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
	GridNode* GetNode(int x, int y);
	void GetNeighbouringNodes(GridNode* centerNode, std::vector<GridNode*>& outNodes);
	void ResetAllNodes();
	void LerpInNodes(float deltaTime);
	void LerpOutNodes(float deltaTime);
	void DisplayHideAllNodes();
	void DisplayShowAllNodes();
};

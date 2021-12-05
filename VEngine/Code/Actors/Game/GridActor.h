#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;
struct TimeOfDayComponent;
struct HealthWidget;
struct IntuitionComponent;
struct GridNode;

//Base struct for every interactive actor that is placed in the level
struct GridActor : Actor
{
	ACTOR_SYSTEM(GridActor);

	MeshComponent* mesh = nullptr;
	HealthWidget* healthWidget = nullptr;
	IntuitionComponent* intuition = nullptr;
	TimeOfDayComponent* timeOfDayComponent = nullptr;

	//Actor's positions on the level grid
	int xIndex = -1;
	int yIndex = -1;

	int health = 0;

	bool isDestructible = true;

	//whether the grid node the gridactor is on is inactive.
	bool isGridObstacle = true;

	bool isInteractable = true;

	std::string interactText;

	GridActor();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
	virtual void InflictDamage(int damage);

	//Sets x and y indices on battlegrid for gridactor
	void SetGridPosition();
	
	//returns the node the gridactor is currently on.
	GridNode* GeCurrentNode();
};

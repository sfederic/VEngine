#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;
struct HealthWidget;
struct Intuition;

//Base struct for every interactive actor that is placed in the level
struct GridActor : Actor
{
	ACTOR_SYSTEM(GridActor);

	MeshComponent* mesh = nullptr;
	HealthWidget* healthWidget = nullptr;

	Intuition* intuition = nullptr;
	std::string intuitionName;

	//Actor's positions on the level grid
	int xIndex = -1;
	int yIndex = -1;

	int health = 0;

	bool isDestructible = false;

	bool isInteractable = false;
	std::string interactText;

	GridActor();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
	void SetIntuition();
};

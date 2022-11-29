#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"
#include "Gameplay/ForwardFace.h"

struct MeshComponent;
struct HealthWidget;
struct MemoryComponent;
struct GridNode;
struct DialogueComponent;

//Base struct for every interactive actor that is placed in the level
struct GridActor : Actor
{
	ACTOR_SYSTEM(GridActor);

	MeshComponent* mesh = nullptr;
	HealthWidget* healthWidget = nullptr;
	MemoryComponent* memoryComponent = nullptr;
	DialogueComponent* dialogueComponent = nullptr;

	//Actor's positions on the level grid
	int xIndex = -1;
	int yIndex = -1;

	int health = 1;

	bool isDestructible = true;

	//whether the grid node the gridactor is on is inactive.
	bool isGridObstacle = true;

	//Physical interaction, moves/activates actor
	bool isInteractable = true;

	//Inspection interaction, shows dialogue
	bool isInspectable = true;

	//Used for when the grid actor is off the main grid, ignores GetNode() calls
	bool disableGridInteract = false;

	bool isInPushback = false;
	XMVECTOR nextPushbackPosition = DirectX::XMVectorZero();
	GridActor* hitActorOnPushback = nullptr;

	std::wstring interactText;
	std::wstring interactKnownText;

	GridActor();

	virtual void Interact() {}

	//Called when this GridActor is attacked.
	virtual void Attacked() {}

	virtual void EndInteract() {}
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
	virtual void InflictDamage(int damage);

	//Sets x and y indices on battlegrid for gridactor
	void SetGridPosition();
	
	//returns the node the gridactor is currently on.
	GridNode* GetCurrentNode();

	void SetAnimation(std::string animationName);

	//Called when the actor is 'pushedback'ed by player or other actor, exploding the actor
	//back into contact with something.
	bool Pushback(XMVECTOR direction);

	//Get forward face in grid terms based on forward vector and current grid position.
	ForwardFace GetCurrentForwardFace();
};

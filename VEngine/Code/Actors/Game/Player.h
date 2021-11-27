#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;
struct CameraComponent;
struct WidgetComponent;
struct Pickup;
struct InteractWidget;

struct Player : Actor
{
	ACTOR_SYSTEM(Player)

	MeshComponent* mesh = nullptr;
	CameraComponent* camera = nullptr;
	WidgetComponent* widget;

	InteractWidget* interactWidget = nullptr;

	Pickup* heldItem = nullptr;

	XMVECTOR nextPos;
	XMVECTOR nextRot;

	bool inConversation = false;
	bool inInteraction = false;
	bool inCombat = false;

	Player();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

private:
	void MovementInput(float deltaTime);
	void RotationInput(float deltaTime);
	void ToggleBattleGrid();
	void PrimaryAction();
};

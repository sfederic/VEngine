#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;
struct CameraComponent;
struct WidgetComponent;
struct IntuitionComponent;
struct Pickup;
struct InteractWidget;
struct IntuitionMenuWidget;
struct PlayerActionBarWidget;
struct Intuition;

struct Player : Actor
{
	ACTOR_SYSTEM(Player)

	MeshComponent* mesh = nullptr;
	CameraComponent* camera = nullptr;
	WidgetComponent* widget;

	InteractWidget* interactWidget = nullptr;
	IntuitionMenuWidget* intuitionMenuWidget = nullptr;
	PlayerActionBarWidget* actionBarWidget = nullptr;

	Pickup* heldItem = nullptr;

	std::map<std::string, Intuition*> intuitions;

	XMVECTOR nextPos;
	XMVECTOR nextRot;

	int actionPoints = 10;

	bool inConversation = false;
	bool inInteraction = false;
	bool inCombat = false;
	bool intuitionWidgetToggle = false;

	Player();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	void CreateIntuition(IntuitionComponent* intuitionComponent, std::string actorAquiredFromName);

private:
	void MovementInput(float deltaTime);
	void RotationInput(float deltaTime);
	void ToggleBattleGrid();
	void PrimaryAction();
	void ToggleIntuitionMenu();
	void ExpendActionPoints(int num);
};

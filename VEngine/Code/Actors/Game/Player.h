#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;
struct CameraComponent;
struct WidgetComponent;
struct DialogueComponent;
struct IntuitionComponent;
struct Pickup;
struct InteractWidget;
struct IntuitionMenuWidget;
struct PlayerActionBarWidget;
struct HeldPickupWidget;
struct PlayerHealthWidget;
struct GuardWidget;
struct Intuition;
struct GridNode;

struct Player : Actor
{
	ACTOR_SYSTEM(Player)

	//Components
	MeshComponent* mesh = nullptr;
	CameraComponent* camera = nullptr;
	DialogueComponent* dialogueComponent = nullptr;

	//Widgets
	InteractWidget* interactWidget = nullptr;
	IntuitionMenuWidget* intuitionMenuWidget = nullptr;
	PlayerActionBarWidget* actionBarWidget = nullptr;
	HeldPickupWidget* heldPickupWidget = nullptr;
	GuardWidget* guardWidget = nullptr;
	PlayerHealthWidget* healthWidget = nullptr;

	Pickup* heldItem = nullptr;

	XMVECTOR nextPos;
	XMVECTOR nextRot;

	int actionPoints = 10;
	int healthPoints = 3;

	int xIndex = -1;
	int yIndex = -1;

	float nextCameraFOV = 0.f;

	bool inConversation = false;
	bool inInteraction = false;
	bool isWeaponDrawn = false;
	bool intuitionWidgetToggle = false;

	bool isPlayerTurn = false;

	bool ableToGuard = false;
	bool guarding = false;
	bool gameOver = false;

	Player();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	//called at every battle turn end
	void RefreshCombatStats();

	GridNode* GetCurrentNode();
	
	void InflictDamage(int damage);

	void Guard();

private:
	void MovementInput(float deltaTime);
	void RotationInput(float deltaTime);
	void ToggleBattleGrid();
	void PrimaryAction();
	void ToggleIntuitionMenu();
	void ExpendActionPoints(int num);
	void LerpPlayerCameraFOV(float deltaTime);
	void CheckNextMoveNode(XMVECTOR previousPos);
	void PlacePickupDown();

	//PrimaryAction actor check functions
	bool PickupCheck(Actor* hitActor);
	bool DialogueCheck(Actor* hitActor);
	bool QuickTalkCheck(Actor* hitActor);
	bool InteractCheck(Actor* hitActor);
	bool DestructibleCheck(Actor* hitActor);
};

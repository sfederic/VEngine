#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;
struct CameraComponent;
struct WidgetComponent;
struct DialogueComponent;
struct MemoryComponent;
struct AudioComponent;
struct InteractWidget;
struct MemoryMenuWidget;
struct PlayerActionBarWidget;
struct PlayerHealthWidget;
struct GuardWidget;
struct Memory;
struct GridNode;

struct Player : Actor
{
	ACTOR_SYSTEM(Player)

	//Components
	MeshComponent* mesh = nullptr;
	CameraComponent* camera = nullptr;

	//Footstep sounds to play while moving
	AudioComponent* stepSounds = nullptr;
	float stepSoundsVolume = 0.f;

	//For dialog files to access
	DialogueComponent* dialogueComponent = nullptr;

	//For player to cache the current dialog file and progress on input
	DialogueComponent* currentlyActiveDialogueComponent = nullptr;

	//Widgets
	//@Todo: I don't like the majority of widgets being here, would rather them in UISystem
	//or some other global UI manager.
	InteractWidget* interactWidget = nullptr;
	MemoryMenuWidget* memoryMenuWidget = nullptr;
	PlayerActionBarWidget* actionBarWidget = nullptr;
	GuardWidget* guardWidget = nullptr;
	PlayerHealthWidget* healthWidget = nullptr;

	XMVECTOR nextPos;
	XMVECTOR nextRot;

	std::string memoryNameToSpawn;

	int actionPoints = 10;
	int healthPoints = 3;

	int xIndex = -1;
	int yIndex = -1;

	float nextCameraFOV = 0.f;

	bool inConversation = false;
	bool inInteraction = false;
	bool isWeaponDrawn = false;
	bool memoryWidgetToggle = false;

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

	//Call on battle end for player variables housekeeping
	void BattleCleanup();
	
	XMVECTOR GetMeshForward();

	//Show a timer dialogue above player when player character is thinking to themself.
	void QuickThought(const std::wstring& text);

private:
	void MovementInput(float deltaTime);
	void RotationInput(float deltaTime);
	void ToggleBattleGrid();
	void PrimaryAction();
	void SecondaryAction();
	void ToggleMemoryMenu();
	void ExpendActionPoints(int num);
	void LerpPlayerCameraFOV(float deltaTime);
	void CheckNextMoveNode(XMVECTOR previousPos);
	void SpawnMemoryAsObject();

	//PrimaryAction actor check functions
	bool DialogueCheck(Actor* hitActor);
	bool QuickTalkCheck(Actor* hitActor);
	bool CombatInteractCheck(Actor* actorToCheck);
	bool InteractCheck(Actor* hitActor);
	bool DestructibleCheck(Actor* hitActor);
};

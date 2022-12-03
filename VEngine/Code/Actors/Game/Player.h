#pragma once

#include "PlayerUnit.h"
#include "Gameplay/BattleEnums.h"

struct CameraComponent;
struct WidgetComponent;
struct DialogueComponent;
struct MemoryComponent;
struct AudioComponent;
struct InteractWidget;
struct MemoryMenuWidget;
struct PlayerHealthWidget;
struct BulletWidget;
struct Memory;
struct GridActor;
struct Unit;

struct Player : public PlayerUnit
{
	ACTOR_SYSTEM(Player)

	//Components
	CameraComponent* camera = nullptr;

	//Footstep sounds to play while moving
	AudioComponent* stepSounds = nullptr;
	float stepSoundsVolume = 0.f;

	//For dialog files to access
	DialogueComponent* dialogueComponent = nullptr;

	//For player to cache the current dialog file and progress on input
	DialogueComponent* currentlyActiveDialogueComponent = nullptr;

	//Widgets
	InteractWidget* interactWidget = nullptr;
	MemoryMenuWidget* memoryMenuWidget = nullptr;
	PlayerHealthWidget* healthWidget = nullptr;
	BulletWidget* bulletWidget = nullptr;

	GridActor* gridActorInteractingWith = nullptr;

	Unit* unitCurrentlyAttackingPlayer = nullptr;

	//Name of the memory selected in MemoryMenuWidget to spawn into world
	std::string memoryNameToSpawn;

	std::vector<Actor*> previousHitTransparentActors;

	int attackPoints = 1;
	int actionPoints = 10;
	int healthPoints = 3;
	int numBullets = 2;

	int xIndex = -1;
	int yIndex = -1;

	float nextCameraFOV = 0.f;

	bool inConversation = false;
	bool inInteraction = false;
	bool isWeaponDrawn = false;
	bool memoryWidgetToggle = false;

	bool isPlayerTurn = true;

	bool ableToGuard = false;
	bool guarding = false;
	bool gameOver = false;

	bool gunModeOn = true;

	Player();
	virtual void Start() override;
	virtual void End() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	//called at every battle turn end
	void RefreshCombatStats();

	void InflictDamage(int damage);

	void Guard();
	void ResetGuard();

	void ResetBulletsToMax();

	//Call on battle end for player variables housekeeping
	void BattleCleanup();
	
	XMVECTOR GetMeshForward();

	//Show a timer dialogue above player when player character is thinking to themself.
	void QuickThought(const std::wstring& text);

private:
	void ToggleBattleGrid();
	void PrimaryAction();
	void SecondaryAction();
	void ToggleMemoryMenu();
	void LerpPlayerCameraFOV(float deltaTime);

	//PrimaryAction actor check functions
	bool DialogueCheck(Actor* hitActor);
	bool QuickTalkCheck(Actor* hitActor);
	bool CombatInteractCheck(Actor* actorToCheck);
	bool InteractCheck(Actor* hitActor);
	bool DestructibleCheck(Actor* hitActor);
	bool AttackGridActorBasedOnNode();
	bool GunShotCheck(Actor* hitActor);
	void PushbackObject();

	bool CheckAttackPositionAgainstUnitDirection(Unit* unit);

	void MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	void ActivateGridMapPicker();

	void PlaceTrap();
	void DisarmTrap();
};

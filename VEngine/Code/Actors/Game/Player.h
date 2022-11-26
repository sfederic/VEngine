#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"
#include "Gameplay/BattleEnums.h"

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
struct BulletWidget;
struct GuardWidget;
struct Memory;
struct GridNode;
struct GridActor;
struct Unit;

struct Player : Actor
{
	ACTOR_SYSTEM(Player)

	AttackDirection attackDirection = AttackDirection::Down;
	DefendDirection defendDirection = DefendDirection::Down;

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
	InteractWidget* interactWidget = nullptr;
	MemoryMenuWidget* memoryMenuWidget = nullptr;
	PlayerActionBarWidget* actionBarWidget = nullptr;
	GuardWidget* guardWidget = nullptr;
	PlayerHealthWidget* healthWidget = nullptr;
	BulletWidget* bulletWidget = nullptr;

	XMVECTOR nextPos;
	XMVECTOR nextRot;

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
	bool guardSuccess = false;
	bool gameOver = false;

	bool gunModeOn = true;

	Player();
	virtual void Start() override;
	virtual void End() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	//called at every battle turn end
	void RefreshCombatStats();

	GridNode* GetCurrentNode();
	
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
	void MovementInput(float deltaTime);
	void RotationInput(float deltaTime);
	void ToggleBattleGrid();
	void PrimaryAction();
	void SecondaryAction();
	void ToggleMemoryMenu();
	void ExpendActionPoints(int num);
	void LerpPlayerCameraFOV(float deltaTime);
	void CheckNextMoveNode(XMVECTOR previousPos);

	bool CheckIfPlayerMovementAndRotationStopped();

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

	void ConfirmGuardOnDirection();

	void PreviewMovementNodesDuringBattle();

	void MakeOccludingMeshBetweenCameraAndPlayerTransparent();
};

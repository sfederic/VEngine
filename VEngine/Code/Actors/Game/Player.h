#pragma once

#include "Actors/Actor.h"
#include "Actors/ActorSystem.h"
#include "Gameplay/BattleEnums.h"
#include "Components/MeshComponent.h"
#include "Core/Log.h"

struct DialogueComponent;
struct MemoryComponent;
struct InteractWidget;
struct MemoryMenuWidget;
struct PlayerHealthWidget;
struct Memory;
struct GridActor;
struct Unit;
struct Trap;
struct MeshComponent;
struct CameraComponent;
struct GridNode;
class Unit;
class GuardWidget;
class PlayerStatusWidget;

class Player : public Actor
{
public:
	ACTOR_SYSTEM(Player);

	DialogueComponent* dialogueComponent = nullptr;
	DialogueComponent* currentlyActiveDialogueComponent = nullptr;

	InteractWidget* interactWidget = nullptr;
	MemoryMenuWidget* memoryMenuWidget = nullptr;
	PlayerHealthWidget* healthWidget = nullptr;

	GridActor* gridActorInteractingWith = nullptr;

	std::vector<Actor*> previousHitTransparentActors;

	bool inConversation = false;
	bool inInteraction = false;
	bool inBattleMode = false;
	bool memoryWidgetToggle = false;
	bool gameOver = false;

	XMVECTOR nextPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR nextRot = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	MeshComponent* mesh = nullptr;
	CameraComponent* camera = nullptr;

	GuardWidget* guardWidget = nullptr;
	PlayerStatusWidget* playerStatusWidget = nullptr;

	int attackPoints = 1;
	int healthPoints = 3;

	float moveSpeed = 0.f;
	float rotateSpeed = 0.f;

	int xIndex = -1;
	int yIndex = -1;

	bool ableToGuard = false;
	bool guarding = false;

	bool isFatigued = false;

	Player();
	void Create() override;
	void Start() override;
	void End() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	//called at every battle turn end
	void RefreshCombatStats();

	//Call on battle end for player variables housekeeping
	void BattleCleanup();
	
	void SetupForBattle();

	XMVECTOR GetMeshForward();

	//Show a timer dialogue above player when player character is thinking to themself.
	void QuickThought(const std::wstring& text);

	void PlaceTrap(Trap* trap);

	void SetGuard();
	void ResetGuard();

	void SetNormalCameraFOV();
	void SetZoomedInCameraFOV();

	void CheckNextMoveNode(XMVECTOR previousPos);
	GridNode* GetCurrentNode();
	void SetGridIndices();
	void GetGridIndices(int& x, int& y);

	virtual void AttackPattern() {};
	void ExpendActionPoint();
	void InflictDamage(int damage);

	void ToggleGridMapPicker(bool& gridPickerActive);

private:
	float nextCameraFOV = 0.f;

	//Toggles battle grid nodes and enters player into a battle ready state.
	void EnterAstralMode();

	bool CheckIfMovementAndRotationStopped();
	void MovementInput(float deltaTime);
	void RotationInput(float deltaTime);

	void PrimaryAction();
	void ToggleMemoryMenu();
	void LerpPlayerCameraFOV(float deltaTime);

	//PrimaryAction actor check functions
	bool DialogueCheck(Actor* hitActor);
	bool QuickTalkCheck(Actor* hitActor);
	bool CombatInteractCheck(Actor* actorToCheck);
	bool InteractCheck(Actor* hitActor);
	bool DestructibleCheck(Actor* hitActor);
	bool AttackGridActorBasedOnNode();

	bool CheckAttackPositionAgainstUnitDirection(Unit* unit);

	void MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	void Guard();
};

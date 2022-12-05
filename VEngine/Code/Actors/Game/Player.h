#pragma once

#include "PlayerUnit.h"
#include <memory>
#include "Gameplay/BattleEnums.h"
#include "Gameplay/BattleCard.h"

struct DialogueComponent;
struct MemoryComponent;
struct InteractWidget;
struct MemoryMenuWidget;
struct PlayerHealthWidget;
struct Memory;
struct GridActor;
struct Unit;

class Player : public PlayerUnit
{
public:
	ACTOR_SYSTEM(Player)

	DialogueComponent* dialogueComponent = nullptr;
	DialogueComponent* currentlyActiveDialogueComponent = nullptr;

	InteractWidget* interactWidget = nullptr;
	MemoryMenuWidget* memoryMenuWidget = nullptr;
	PlayerHealthWidget* healthWidget = nullptr;

	GridActor* gridActorInteractingWith = nullptr;
	Unit* unitCurrentlyAttackingPlayer = nullptr;

	std::vector<Actor*> previousHitTransparentActors;

	std::vector<std::unique_ptr<BattleCard>> battleCardsInHand;
	
	float nextCameraFOV = 0.f;

	bool inConversation = false;
	bool inInteraction = false;
	bool isWeaponDrawn = false;
	bool memoryWidgetToggle = false;
	bool isPlayerTurn = true;
	bool gameOver = false;

	Player();
	virtual void Start() override;
	virtual void End() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	//called at every battle turn end
	void RefreshCombatStats();

	void InflictDamage(int damage);

	//Call on battle end for player variables housekeeping
	void BattleCleanup();
	
	XMVECTOR GetMeshForward();

	//Show a timer dialogue above player when player character is thinking to themself.
	void QuickThought(const std::wstring& text);

private:
	void ToggleBattleGrid();
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
	void PushbackObject();

	bool CheckAttackPositionAgainstUnitDirection(Unit* unit);

	void MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	void ActivateGridMapPicker();

	void PlaceTrap();
	void DisarmTrap();

	void SummonAllyUnit();
	void SwitchInputBetweenAllyUnitsAndPlayer();

	void DrawBattleCard();

	std::vector<PlayerUnit*> activePlayerUnits;
	int activePlayerUnitIndex = 0;
};

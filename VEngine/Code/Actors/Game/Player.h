#pragma once

#include "PlayerUnit.h"
#include "Gameplay/BattleEnums.h"

struct DialogueComponent;
struct MemoryComponent;
struct InteractWidget;
struct MemoryMenuWidget;
struct PlayerHealthWidget;
struct Memory;
struct GridActor;
struct Unit;
class BattleCardHandWidget;
struct BattleCard;

class Player : public PlayerUnit
{
public:
	ACTOR_SYSTEM(Player)

	DialogueComponent* dialogueComponent = nullptr;
	DialogueComponent* currentlyActiveDialogueComponent = nullptr;

	InteractWidget* interactWidget = nullptr;
	MemoryMenuWidget* memoryMenuWidget = nullptr;
	PlayerHealthWidget* healthWidget = nullptr;
	BattleCardHandWidget* battleCardHandWidget = nullptr;

	GridActor* gridActorInteractingWith = nullptr;

	std::vector<Actor*> previousHitTransparentActors;

	std::vector<BattleCard*> battleCardsInHand;
	
	float nextCameraFOV = 0.f;

	bool inConversation = false;
	bool inInteraction = false;
	bool inAstralMode = false;
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

	//Call on battle end for player variables housekeeping
	void BattleCleanup();
	
	void SetupForBattle();

	XMVECTOR GetMeshForward();

	//Show a timer dialogue above player when player character is thinking to themself.
	void QuickThought(const std::wstring& text);

	void DrawTurnBattleCardHand();

	void SummonAllyUnit();

	void PlaceTrap(BattleCard* trapCard);

private:
	//Toggles battle grid nodes and enters player into a battle ready state.
	void EnterAstralMode();

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

	void SwitchInputBetweenAllyUnitsAndPlayer();

	void DrawBattleCard();
	void ActivateFirstBattleCardInHand();

	std::vector<PlayerUnit*> activePlayerUnits;
	int activePlayerUnitIndex = 0;
	const static int battleCardHandSizeLimit = 5;
};

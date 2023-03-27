#pragma once

#include "PlayerUnit.h"
#include "Actors/ActorSystem.h"
#include "Gameplay/BattleEnums.h"
#include "Gameplay/PlayerInputController.h"
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
class BattleCardHandWidget;
struct TrapCard;
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
	bool gameOver = false;

	Player();
	virtual void Start() override;
	virtual void End() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	template <typename AllyUnitType>
	void SummonAllyUnit()
	{
		static_assert(std::is_base_of<PlayerUnit, AllyUnitType>() == true);

		if (activePlayerUnits.size() == 3)
		{
			Log("Cannot summon any more ally units. Max is 3.");
			return;
		}

		Transform transform;
		XMStoreFloat3(&transform.position, GetPositionV() + GetForwardVectorV());
		transform.rotation = mesh->GetWorldRotation();

		auto attackUnit = AllyUnitType::system.Add(transform);
		attackUnit->Start();

		playerInputController.SetPlayerUnitToControl(attackUnit);

		activePlayerUnits.push_back(attackUnit);
		activePlayerUnitIndex = activePlayerUnits.size() - 1;
		playerInputController.SetPlayerUnitToControl(attackUnit);
	}

	//called at every battle turn end
	void RefreshCombatStats();

	//Call on battle end for player variables housekeeping
	void BattleCleanup();
	
	void SetupForBattle();

	XMVECTOR GetMeshForward();

	//Show a timer dialogue above player when player character is thinking to themself.
	void QuickThought(const std::wstring& text);

	void DrawTurnBattleCardHand();

	void PlaceTrap(TrapCard* trapCard);

	//Currently more of a debug function (Hand limit isn't considered)
	void AddCardToHand(BattleCard* card);

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

	bool CheckAttackPositionAgainstUnitDirection(Unit* unit);

	void MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	void SwitchInputBetweenAllyUnitsAndPlayer();

	void DrawBattleCard();
	void ActivateFirstBattleCardInHand();

	void SpawnPhysicalRepresentationOfAstralPlayer();
	void DestroyPlayerPhysicalBodyDoubleAndReturnPlayerPosition();
	MeshComponent* playerBodyMesh = nullptr;

	std::vector<PlayerUnit*> activePlayerUnits;
	int activePlayerUnitIndex = 0;
	const static int battleCardHandSizeLimit = 5;
};

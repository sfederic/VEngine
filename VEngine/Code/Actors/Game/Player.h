#pragma once

#include "Actors/Actor.h"
#include "Actors/ActorSystem.h"

struct DialogueComponent;
struct InteractWidget;
struct PlayerHealthWidget;
class PickupWidget;
struct GridActor;
struct Unit;
struct MeshComponent;
struct CameraComponent;
struct GridNode;
class Unit;

class Player : public Actor
{
public:
	ACTOR_SYSTEM(Player);

	DialogueComponent* dialogueComponent = nullptr;
	DialogueComponent* currentlyActiveDialogueComponent = nullptr;

	InteractWidget* interactWidget = nullptr;
	PlayerHealthWidget* healthWidget = nullptr;

	GridActor* gridActorInteractingWith = nullptr;

	std::vector<Actor*> previousHitTransparentActors;

	bool inConversation = false;
	bool inInteraction = false;
	bool inInspection = false;
	bool gameOver = false;

	XMVECTOR nextPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR nextRot = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	MeshComponent* mesh = nullptr;
	CameraComponent* camera = nullptr;

	int healthPoints = 3;

	float moveSpeed = 0.f;
	float rotateSpeed = 0.f;

	int xIndex = -1;
	int yIndex = -1;

	Player();
	void Create() override;
	void Start() override;
	void End() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	XMVECTOR GetMeshForward();
	XMVECTOR GetMeshRight();

	//Show a timer dialogue above player when player character is thinking to themself.
	void QuickThought(const std::wstring& text);

	void SetDefaultCameraFOV();
	void SetZoomedInCameraFOV();

	void CheckNextMoveNode(XMVECTOR previousPos);
	GridNode* GetCurrentNode();
	void SetGridIndices();
	void GetGridIndices(int& x, int& y);

	void SetInteractWidgetText(std::wstring_view interactText);

	void SetLinkedGridActor(GridActor& gridActor);
	void ResetLinkedGridActor();
	void ResetLinkedGridActorIfThis(GridActor* gridActor);

	void SetNextPosAndRotToCurrent();

	void SetPickupWidgetIconFilename(std::string_view filename);
	void AddPickupWidgetToViewport();
	void RemovePickupWidgetFromViewport();

	void SetNextCameraPosition(XMVECTOR pos) { nextCameraPosition = pos; }
	void ResetCameraPosAndTargetToPlayer();
	void SetCameraTargetActor(Actor* target);

private:
	PickupWidget* pickupWidget = nullptr;

	XMVECTOR nextCameraPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR cameraLinkActiveLocalPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR cameraStartingLocalPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	float nextCameraFOV = 0.f;

	bool isInputLinkedToGridActor = false;
	GridActor* linkedGridActor = nullptr;

	bool CheckIfMovementAndRotationStopped();
	void MovementInput(float deltaTime);
	void RotationInput(float deltaTime);

	void PrimaryAction();
	void LerpPlayerCameraFOV(float deltaTime);

	//PrimaryAction actor check functions
	bool DialogueCheck(Actor* hitActor);
	bool QuickTalkCheck(Actor* hitActor);
	bool CombatInteractCheck(Actor* actorToCheck);

	bool InteractCheck(Actor* hitActor);
	void InteractInfoToWidgetCheck();

	bool DestructibleCheck(Actor* hitActor);
	bool AttackGridActorBasedOnNode();

	bool CheckAttackPositionAgainstUnitDirection(Unit* unit);

	void MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	void LinkToGridActor();
	void MoveLinkedGridActor();
	void RotateLinkedGridActor();

	void ToggleGrid();

	void HighlightLinkableGridActor();
	GridActor* highlightedGridActor = nullptr;

	void OverlapPickupGridActor();

	void ResetHighlightedActor();
};

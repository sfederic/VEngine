#pragma once

#include "Actors/Actor.h"
#include "Actors/ActorSystem.h"
#include <set>

struct DialogueComponent;
class InteractWidget;
struct PlayerHealthWidget;
class GridActor;
class MeshComponent;
class CameraComponent;
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

	std::set<Actor*> previousHitTransparentActors;

	bool inConversation = false;
	bool inInteraction = false;
	bool inInspection = false;
	bool gameOver = false;

	XMVECTOR nextPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR nextRot = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	MeshComponent* mesh = nullptr;
	MeshComponent* linkEffectMesh = nullptr;
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

	void SetDefaultCameraFOV();
	void SetZoomedInCameraFOV();

	void CheckNextMoveNode(const XMVECTOR previousPos);
	GridNode* GetCurrentNode();
	void SetGridIndices();
	void GetGridIndices(int& x, int& y);

	void SetInteractWidgetText(std::wstring_view interactText);

	void SetLinkedGridActor(GridActor& gridActor);
	void ResetLinkedGridActor();
	void ResetLinkedGridActorIfThis(GridActor* gridActor);

	void SetNextPosAndRotToCurrent();

	void SetNextCameraPosition(XMVECTOR pos) { nextCameraPosition = pos; }
	XMVECTOR GetCameraLocalPosition();
	void ResetCameraPosAndTargetToPlayer();
	void SetCameraTargetActor(Actor* target);
	Actor& GetCameraTargetActor();

	void SetQuickThought(std::wstring_view text);

	void SetLinkEffectMeshForTransformativeGridActors(MeshComponent& mesh);

private:
	XMVECTOR nextCameraPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR cameraLinkActiveLocalPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR cameraStartingLocalPosition = XMVectorSet(1.75f, 1.75f, -2.75f, 1.f);
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

	bool AttackGridActorBasedOnNode();

	bool CheckAttackPositionAgainstUnitDirection(Unit* unit);

	void MakeOccludingMeshBetweenCameraAndPlayerTransparent();
	bool CheckIfPlayerIsInsidePlayerCameraTriggerAndReset();

	void LinkToGridActor();
	void MoveLinkedGridActor();
	void RotateLinkedGridActor();

	void ToggleGrid();

	void HighlightLinkableGridActor();
	GridActor* highlightedGridActor = nullptr;
	GridActor* previousHighlightedGridActor = nullptr;

	void ResetHighlightedActor();

	void OnMoveAndRotateEnd();

	bool IsInInteraction() const;

	//Link effect logic to use on grid actor link select hover.
	void EnableLinkEffectMeshForHover(MeshComponent* mesh);
	//Link effect logic to use on grid actor link select.
	void EnableLinkEffectMeshForSelect(MeshComponent* mesh);
	void UpdateLinkEffectMesh();
	bool linkEffectMeshSetAlpha = false;

	void PlayFootstepAudio() const;

	bool CheckIfMeshCanBeLinkedTo(GridActor* gridActorToLinkTo);

	bool previousMovementAndRotationStoppedValue = false;

	bool playerInQuickThought = false;
};

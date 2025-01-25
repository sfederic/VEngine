#pragma once

#include "Actors/Actor.h"
#include "Actors/ActorSystem.h"
#include <set>

class DialogueComponent;
class InteractWidget;
class PlayerUpgradeInfoWidget;
class ReconWidget;
class GridActor;
class MeshComponent;
class CameraComponent;
struct GridNode;
class Unit;

class Player : public Actor
{
public:
	ACTOR_SYSTEM(Player);

	DirectX::XMVECTOR nextPos = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	DirectX::XMVECTOR nextRot = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	DirectX::XMVECTOR nextHopPos = DirectX::XMVectorZero();

private:
	DirectX::XMVECTOR nextCameraLocalPosition = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	DirectX::XMVECTOR cameraLinkActiveLocalPosition = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
	DirectX::XMVECTOR cameraStartingLocalPosition = DirectX::XMVectorSet(1.75f, 1.75f, -2.75f, 1.f);
	DirectX::XMVECTOR cameraReconModeLocalPosition = DirectX::XMVectorSet(0.f, 0.25f, 0.15f, 1.f);

public:
	std::set<Actor*> previousHitTransparentActors;

	DialogueComponent* dialogueComponent = nullptr;
	DialogueComponent* currentlyActiveDialogueComponent = nullptr;

	InteractWidget* interactWidget = nullptr;
	ReconWidget* reconWidget = nullptr;
	PlayerUpgradeInfoWidget* upgradeInfoWidget = nullptr;

	GridActor* gridActorInteractingWith = nullptr;

	MeshComponent* _mesh = nullptr;
	MeshComponent* linkEffectMesh = nullptr;
	CameraComponent* camera = nullptr;

	int healthPoints = 3;

	float moveSpeed = 0.f;
	float rotateSpeed = 0.f;

	int xIndex = -1;
	int yIndex = -1;

	bool inConversation = false;
	bool inInteraction = false;
	bool inInspection = false;
	bool gameOver = false;

	Player();
	void Create() override;
	void Start() override;
	void End() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	DirectX::XMVECTOR GetMeshForward();
	DirectX::XMVECTOR GetMeshRight();

	void SetDefaultCameraFOV();
	void SetZoomedInCameraFOV();

	void CheckNextMoveNode(const DirectX::XMVECTOR previousPos);
	GridNode* GetCurrentNode();
	void SetGridIndices();
	void GetGridIndices(int& x, int& y);

	void SetInteractWidgetText(std::wstring_view interactText);

	void SetLinkedGridActor(GridActor& gridActor);
	void ResetLinkedGridActor();
	void ResetLinkedGridActorIfThis(GridActor* gridActor);

	void SetNextPosAndRotToCurrent();

	void SetNextCameraPosition(DirectX::XMVECTOR pos) { nextCameraLocalPosition = pos; }
	DirectX::XMVECTOR GetCameraLocalPosition();
	void ResetCameraPosAndTargetToPlayer();
	void SetCameraTargetActor(Actor* target);
	Actor& GetCameraTargetActor();

	void SetQuickThought(std::wstring_view text);

	void SetLinkEffectMeshForTransformativeGridActors(MeshComponent& mesh);

private:
	float nextCameraFOV = 0.f;
	int cameraZoomLevel = 0;

	bool isInputLinkedToGridActor = false;
	GridActor* linkedGridActor = nullptr;

	bool CheckIfMovementAndRotationStopped();
	void MovementInput(float deltaTime);
	void RotationInput(float deltaTime);

	void PrimaryAction();
	void LerpPlayerCameraFOV(float deltaTime);
	void ZoomCameraInAndOut();

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

	void ToggleReconMode();
	void DisplayReconInfo();
	void ReconSnapshot();
	bool reconModeOn = false;

	void ToggleUpgradeInfoWidget();

	bool CheckIfUpgradeExists(std::string upgradeName);

	//Link effect logic to use on grid actor link select hover.
	void EnableLinkEffectMeshForHover(MeshComponent* linkMesh);
	//Link effect logic to use on grid actor link select.
	void EnableLinkEffectMeshForSelect(MeshComponent* mesh);
	void UpdateLinkEffectMesh();
	bool linkEffectMeshSetAlpha = false;

	void PlayFootstepAudio() const;

	bool CheckIfMeshCanBeLinkedTo(GridActor* gridActorToLinkTo);

	//Cute little hop animation when jumping off higher nodes. Think like Pokémon Blue when you jump.
	void CuteHopToLowerNode(const DirectX::XMFLOAT3 nextNodePos);
	bool inHop = false;
	bool hopLanding = false;

	bool previousMovementAndRotationStoppedValue = false;

	bool playerInQuickThought = false;
};

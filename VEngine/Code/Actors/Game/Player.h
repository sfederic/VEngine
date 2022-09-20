#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct CameraComponent;
class ScanWidget;
class PhotoWidget;
class DialogueWidget;
class SalvageMissionWidget;
class PlayerActionBarWidget;

class Player : public Actor
{
public:
	ACTOR_SYSTEM(Player);

	Player();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	void StartDialogue(std::string dialogueFilename);
	void SetInCombat(bool combatActive);

private:
	void MovementInput(float deltaTime);
	bool CheckIfPlayerMovementAndRotationStopped();
	void SetMovementAxis();
	bool RotatePlayerOnWallMoveHit(XMVECTOR movementAxis, XMVECTOR raycastAxis,
		float rightAngle, float forwardAngle, float leftAngle, float backAngle);
	void ShootInput();
	void Interact();
	bool IsFloorEmptyOnNextMove();
	void Scan();
	void TakePhoto();
	void RaycastAgainstActorWithPhotoComponent();
	void ScanVisorInputToggle();
	void CreatePlayerWidgets();
	void SpawnNote();
	void ToggleSalvageMissionStats();
	void ProgressDialogue();
	void EndDialogue();
	bool CombatMoveCheck();
	void EndCombatTurn();

public:
	CameraComponent* camera = nullptr;

private:
	ScanWidget* scanWidget = nullptr;
	PhotoWidget* photoWidget = nullptr;
	SalvageMissionWidget* salvageMissionWidget = nullptr;
	DialogueWidget* dialogueWidget = nullptr;
	PlayerActionBarWidget* actionBarWidget = nullptr;

	XMVECTOR movementAxes[4]{};

	XMVECTOR forwardAxis = XMVectorZero();
	XMVECTOR rightAxis = XMVectorZero();

	XMVECTOR nextPos = XMVectorZero();
	XMVECTOR nextRot = XMVectorZero();

	float moveSpeed = 3.f;
	float rotSpeed = 2.5f;

	static const int MAX_ACTION_POINTS = 7;
	int combatActionPoints = MAX_ACTION_POINTS;
	bool inCombat = false;

	bool scanVisorActive = false;
	bool shakeOnWallRotateEnd = false;
	bool salvageMissionMenuOpen = false;
};

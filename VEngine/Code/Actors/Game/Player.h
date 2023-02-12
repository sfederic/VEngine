#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class MeshComponent;
class CameraComponent;
class PlayerShieldWidget;

class Player : public Actor
{
public:
	ACTOR_SYSTEM(Player)

	Player();
	virtual void Create() override;
	virtual void Start() override;
	virtual void End() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	void InflictDamage(float damage);

private:
	void MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	void MovementInput();
	void RotationInput();

	bool CheckMovementAndRotationHaveStopped();
	bool CheckPlayerWithinLevelBounds();
	bool CheckForObstacle();

	void Shoot();
	void BladeSwipe();
	void ShieldLogic(float deltaTime);
	void Interact();

public:
	MeshComponent* mesh = nullptr;
	CameraComponent* camera = nullptr;

	float shields = 100.f;

private:
	PlayerShieldWidget* playerShieldWidget = nullptr;

	MeshComponent* shieldMesh = nullptr;

	//When to turn shield effect off after interval of damage.
	float shieldCountdownTimer = 0.5f;

	std::vector<Actor*> previousHitTransparentActors;

	XMVECTOR nextPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR nextRot = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	float movementSpeed = 10.f;
	float rotationSpeed = 10.f;

	bool allRangeModeActive = true;
	bool inDashBladeAttack = false;

	int damage = 1;
};

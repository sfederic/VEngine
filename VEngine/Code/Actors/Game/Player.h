#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class MeshComponent;
class CameraComponent;
class CharacterControllerComponent;
class MouseRotateComponent;
class PlayerShieldWidget;
class PlayerReticleWidget;
class EquippedGearsWidget;
class Gear;

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
	auto GetDamage() { return damage; }

	XMVECTOR GetAimDirection();

protected:
	void MovementInput(float deltaTime);
	void SetCameraAsActive();
	void SetMouseInputForCameraRotation(bool useInput);

private:
	void MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	void PrimaryGearAction();
	void SecondaryGearAction();
	void ShieldLogic(float deltaTime);
	void Interact();

	void SetEquippedGears();

	void LockOnTarget();

	void LandToAirFormChange();

public:
	MeshComponent* mesh = nullptr;
	CameraComponent* camera = nullptr;

	float shields = 100.f;

private:
	PlayerShieldWidget* playerShieldWidget = nullptr;
	PlayerReticleWidget* reticleWidget = nullptr; 
	EquippedGearsWidget* equippedGearsWidget = nullptr;

	MeshComponent* shieldMesh = nullptr;

	CharacterControllerComponent* characterController = nullptr;
	MouseRotateComponent* mouseRotateComponent = nullptr;

	Gear* primaryGear = nullptr;
	Gear* secondaryGear = nullptr;

	//When to turn shield effect off after interval of damage.
	float shieldCountdownTimer = 0.5f;

	std::vector<Actor*> previousHitTransparentActors;

	XMVECTOR reticleWidgetNextPos = XMVectorZero();
	float reticleWidgetLerpValue = 0.f;

	float movementSpeed = 0.f;
	float rotationSpeed = 10.f;

	int damage = 1;

	bool allRangeModeActive = true;
	bool inDashBladeAttack = false;

	bool isMoving = false;
	XMVECTOR movementDirection = XMVectorZero();

	Actor* lockOnActor = nullptr;
};

#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;
struct CameraComponent;
struct GridNode;

//Base class for all player controllable units.
//Should never be instantiated, no need for ACTOR_SYSTEM.
class PlayerUnit : public Actor
{
public:
	PlayerUnit();

	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	Properties GetProps() { return __super::GetProps(); }

	void ControllerInput(float deltaTime);

	void CheckNextMoveNode(XMVECTOR previousPos);
	GridNode* GetCurrentNode();

	bool CheckAndExpendActionPoints(int num);
	void InflictDamage(int damage);

	void ToggleGridMapPicker(bool& gridPickerActive);

protected:
	void PreviewMovementNodesDuringBattle();

	bool CheckIfMovementAndRotationStopped();
	void MovementInput(float deltaTime);
	void RotationInput(float deltaTime);

	void FuseWithAllyUnit();

private:
	void Attack();

public:
	XMVECTOR nextPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR nextRot = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	MeshComponent* mesh = nullptr;
	CameraComponent* camera = nullptr;

	int attackPoints = 1;
	int healthPoints = 1;

	float moveSpeed = 5.f;
	float rotSpeed = 3.5f;

	int xIndex = -1;
	int yIndex = -1;

	bool isMainPlayer = false;
};

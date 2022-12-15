#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;
class CameraComponent;

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

private:
	void MakeOccludingMeshBetweenCameraAndPlayerTransparent();

	void MovementInput();
	void RotationInput();
	bool CheckMovementAndRotationHaveStopped();

	void Shoot();
	void BladeSwipe();

public:
	MeshComponent* mesh = nullptr;
	CameraComponent* camera = nullptr;

private:
	std::vector<Actor*> previousHitTransparentActors;

	XMVECTOR nextPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR nextRot = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	float movementSpeed = 9.5f;
	float rotationSpeed = 5.5f;
};

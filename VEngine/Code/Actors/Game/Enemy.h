#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct BoxTriggerComponent;
struct MeshComponent;

class Enemy : public Actor
{
public:
	ACTOR_SYSTEM(Enemy);

	Enemy();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

private:
	void PlayerEnteredAggroTrigger();

	//Trigger that shows the enemy's aggro field.
	BoxTriggerComponent* aggroTrigger = nullptr;

	MeshComponent* mesh = nullptr;

	bool inCombat = false;
};

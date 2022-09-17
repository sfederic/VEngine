#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct BoxTriggerComponent;
class LevelEntranceWidget;

//Used to enter levels from overworld using PlayerShip.
class LevelEntranceTrigger : public Actor
{
public:
	ACTOR_SYSTEM(LevelEntranceTrigger);

	LevelEntranceTrigger();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

private:
	BoxTriggerComponent* boxTriggerComponent = nullptr;

	LevelEntranceWidget* levelEntranceWidget = nullptr;

	std::wstring levelName;
};

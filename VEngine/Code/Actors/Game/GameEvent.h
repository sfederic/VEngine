#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"
#include <vector>

struct EmptyComponent;
struct Actor;

//Game events are an in-world instance that map to a 'time of day' during gameplay,
//linking actors to be activated in a game-time timeframe.
struct GameEvent : Actor
{
	ACTOR_SYSTEM(GameEvent)

	EmptyComponent* root = nullptr;

	//Times to start and end event
	int startHour = 0;
	int endHour = 0;
	int startMinute = 0;
	int endMinute = 0;

	std::vector<Actor*> actorsToActivate;

	GameEvent();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};

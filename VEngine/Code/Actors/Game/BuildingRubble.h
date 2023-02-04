#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BuildingRubble : public Actor
{
public:
	ACTOR_SYSTEM(BuildingRubble);

	BuildingRubble();
	void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

private:
	class MeshComponent* mesh = nullptr;
};
